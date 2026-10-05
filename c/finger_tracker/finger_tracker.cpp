// Real-time finger tracker using MediaPipe's HandLandmarker model.
// C++ port of finger_tracker.py, built on MediaPipe's C API + OpenCV.
//
// Controls:
//     q / Esc  quit
//     d        toggle air-drawing with the index fingertip
//     c        clear the drawing
//     l        toggle landmark skeleton

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <deque>
#include <filesystem>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <opencv2/opencv.hpp>

#include "mediapipe_c.h"

namespace fs = std::filesystem;

// Landmark indices (see MediaPipe hand landmark model docs)
constexpr int WRIST = 0;
constexpr int THUMB_TIP = 4, THUMB_IP = 3;
const std::vector<std::pair<std::string, int>> FINGER_TIPS = {
    {"Index", 8}, {"Middle", 12}, {"Ring", 16}, {"Pinky", 20}};
const std::map<std::string, int> FINGER_PIPS = {
    {"Index", 6}, {"Middle", 10}, {"Ring", 14}, {"Pinky", 18}};
constexpr int INDEX_TIP = 8;

const std::vector<std::pair<int, int>> HAND_CONNECTIONS = {
    // palm
    {0, 1}, {1, 5}, {9, 13}, {13, 17}, {5, 9}, {0, 17},
    // thumb
    {1, 2}, {2, 3}, {3, 4},
    // index
    {5, 6}, {6, 7}, {7, 8},
    // middle
    {9, 10}, {10, 11}, {11, 12},
    // ring
    {13, 14}, {14, 15}, {15, 16},
    // pinky
    {17, 18}, {18, 19}, {19, 20},
};

const std::vector<std::pair<int, cv::Scalar>> TIP_COLORS = {  // BGR
    {THUMB_TIP, {255, 128, 0}},
    {8, {0, 255, 0}},
    {12, {0, 255, 255}},
    {16, {255, 0, 255}},
    {20, {0, 128, 255}},
};

// Throws on a non-OK status, freeing the C-allocated error message.
void check(MpStatus status, char* error_msg, const char* what) {
  if (status == MP_OK) return;
  std::string msg = std::string(what) + " failed (status " + std::to_string(status) + ")";
  if (error_msg) {
    msg += ": ";
    msg += error_msg;
    MpErrorFree(error_msg);
  }
  throw std::runtime_error(msg);
}

class HandLandmarker {
 public:
  HandLandmarker(const std::string& model_path, int num_hands) {
    MpHandLandmarkerOptions options{};
    options.base_options.model_asset_path = model_path.c_str();
    options.base_options.file_descriptor = -1;
    options.base_options.host_system = kMpHostSystemLinux;
    options.running_mode = kMpRunningModeVideo;
    options.num_hands = num_hands;
    options.min_hand_detection_confidence = 0.5f;
    options.min_hand_presence_confidence = 0.5f;
    options.min_tracking_confidence = 0.5f;
    char* err = nullptr;
    check(MpHandLandmarkerCreate(&options, &handle_, &err), err, "MpHandLandmarkerCreate");
  }

  ~HandLandmarker() {
    char* err = nullptr;
    if (MpHandLandmarkerClose(handle_, &err) != MP_OK && err) MpErrorFree(err);
  }

  HandLandmarker(const HandLandmarker&) = delete;
  HandLandmarker& operator=(const HandLandmarker&) = delete;

  // `rgb` must be a continuous CV_8UC3 RGB image. The caller must pass the
  // returned result to MpHandLandmarkerCloseResult.
  MpHandLandmarkerResult detect_for_video(const cv::Mat& rgb, int64_t timestamp_ms) {
    char* err = nullptr;
    MpImagePtr image = nullptr;
    check(MpImageCreateFromUint8Data(kMpImageFormatSrgb, rgb.cols, rgb.rows, rgb.data,
                                     static_cast<int>(rgb.total() * rgb.elemSize()), &image,
                                     &err),
          err, "MpImageCreateFromUint8Data");

    MpImageProcessingOptions proc{};
    MpHandLandmarkerResult result{};
    MpStatus status =
        MpHandLandmarkerDetectForVideo(handle_, image, &proc, timestamp_ms, &result, &err);
    MpImageFree(image);
    check(status, err, "MpHandLandmarkerDetectForVideo");
    return result;
  }

 private:
  MpHandLandmarkerPtr handle_ = nullptr;
};

std::vector<cv::Point> to_pixels(const MpNormalizedLandmarks& lms, int width, int height) {
  std::vector<cv::Point> pts;
  pts.reserve(lms.landmarks_count);
  for (uint32_t i = 0; i < lms.landmarks_count; ++i) {
    pts.emplace_back(static_cast<int>(lms.landmarks[i].x * width),
                     static_cast<int>(lms.landmarks[i].y * height));
  }
  return pts;
}

// Return names of fingers that are extended.
//
// The frame is mirrored before detection, so MediaPipe's handedness label
// matches the user's real hand.
std::vector<std::string> raised_fingers(const MpNormalizedLandmarks& lms,
                                        const std::string& handedness) {
  std::vector<std::string> up;
  const auto* lm = lms.landmarks;
  // Thumb: compare x of tip vs IP joint; direction depends on which hand.
  float tip_x = lm[THUMB_TIP].x, ip_x = lm[THUMB_IP].x;
  if ((handedness == "Right" && tip_x < ip_x) || (handedness == "Left" && tip_x > ip_x))
    up.push_back("Thumb");
  // Other fingers: tip above PIP joint (smaller y) means extended.
  for (const auto& [name, tip] : FINGER_TIPS) {
    if (lm[tip].y < lm[FINGER_PIPS.at(name)].y) up.push_back(name);
  }
  return up;
}

bool contains(const std::vector<std::string>& v, const std::string& s) {
  return std::find(v.begin(), v.end(), s) != v.end();
}

std::string join(const std::vector<std::string>& v, const std::string& sep) {
  std::string out;
  for (size_t i = 0; i < v.size(); ++i) out += (i ? sep : "") + v[i];
  return out;
}

void draw_hand(cv::Mat& frame, const std::vector<cv::Point>& points, bool show_skeleton) {
  if (show_skeleton) {
    for (const auto& [a, b] : HAND_CONNECTIONS)
      cv::line(frame, points[a], points[b], {200, 200, 200}, 2);
    for (const auto& p : points) cv::circle(frame, p, 3, {255, 255, 255}, -1);
  }
  for (const auto& [idx, color] : TIP_COLORS) {
    cv::circle(frame, points[idx], 9, color, -1);
    cv::circle(frame, points[idx], 9, {0, 0, 0}, 1);
  }
}

void put_label(cv::Mat& frame, const std::string& text, cv::Point org, double scale = 0.6,
               cv::Scalar color = {255, 255, 255}) {
  cv::putText(frame, text, org, cv::FONT_HERSHEY_SIMPLEX, scale, {0, 0, 0}, 4, cv::LINE_AA);
  cv::putText(frame, text, org, cv::FONT_HERSHEY_SIMPLEX, scale, color, 1, cv::LINE_AA);
}

int run(const fs::path& model_path) {
  if (!fs::exists(model_path)) {
    std::fprintf(stderr, "Model not found: %s\n", model_path.c_str());
    return 1;
  }

  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::fprintf(stderr, "Could not open the camera.\n");
    return 1;
  }
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

  HandLandmarker landmarker(model_path.string(), 2);
  cv::Mat canvas;
  constexpr size_t TRAIL_LEN = 32;  // short fading trail behind the index tip
  std::deque<std::optional<cv::Point>> trail;
  std::optional<cv::Point> prev_draw_point;
  bool drawing_mode = false;
  bool show_skeleton = true;

  using Clock = std::chrono::steady_clock;
  const auto start = Clock::now();
  int64_t last_ts = -1;
  double fps = 0.0;
  auto prev_time = Clock::now();

  const std::string window = "Finger Tracker";
  cv::namedWindow(window, cv::WINDOW_NORMAL);

  cv::Mat frame, rgb;
  char buf[128];
  while (true) {
    if (!cap.read(frame) || frame.empty()) break;
    cv::flip(frame, frame, 1);  // mirror view
    const int h = frame.rows, w = frame.cols;
    if (canvas.empty()) canvas = cv::Mat::zeros(frame.size(), frame.type());

    cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
    int64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count();
    ts = std::max(ts, last_ts + 1);  // timestamps must strictly increase
    last_ts = ts;
    MpHandLandmarkerResult result = landmarker.detect_for_video(rgb, ts);
    const uint32_t num_hands = result.hand_landmarks_count;

    std::optional<cv::Point> index_tip;
    for (uint32_t i = 0; i < num_hands; ++i) {
      const MpNormalizedLandmarks& lms = result.hand_landmarks[i];
      if (lms.landmarks_count < 21) continue;
      std::string handedness = "Unknown";
      if (i < result.handedness_count && result.handedness[i].categories_count > 0 &&
          result.handedness[i].categories[0].category_name)
        handedness = result.handedness[i].categories[0].category_name;

      auto points = to_pixels(lms, w, h);
      draw_hand(frame, points, show_skeleton);

      auto up = raised_fingers(lms, handedness);
      cv::Point wrist = points[WRIST];
      put_label(frame, handedness + ": " + std::to_string(up.size()) + " up",
                {wrist.x - 40, wrist.y + 30}, 0.7, {0, 255, 255});
      put_label(frame, up.empty() ? "fist" : join(up, ", "), {wrist.x - 40, wrist.y + 55}, 0.5);

      if (i == 0) {
        cv::Point tip = points[INDEX_TIP];
        index_tip = tip;
        std::snprintf(buf, sizeof(buf), "(%d, %d)", tip.x, tip.y);
        put_label(frame, buf, {tip.x + 12, tip.y - 12}, 0.5, {0, 255, 0});
        // Draw only while the index finger alone (or with thumb) is raised
        bool pen_down = drawing_mode && contains(up, "Index") && !contains(up, "Middle");
        if (pen_down) {
          if (prev_draw_point) cv::line(canvas, *prev_draw_point, tip, {0, 255, 0}, 6, cv::LINE_AA);
          prev_draw_point = tip;
        } else {
          prev_draw_point.reset();
        }
      }
    }
    MpHandLandmarkerCloseResult(&result);

    if (!index_tip) prev_draw_point.reset();
    trail.push_back(index_tip);
    if (trail.size() > TRAIL_LEN) trail.pop_front();

    // Fading trail
    for (size_t j = 1; j < trail.size(); ++j) {
      if (!trail[j - 1] || !trail[j]) continue;
      int thickness = std::max(1, static_cast<int>(8 * j / trail.size()));
      cv::line(frame, *trail[j - 1], *trail[j], {0, 200, 0}, thickness, cv::LINE_AA);
    }

    // Overlay the drawing canvas
    cv::Mat any_channel;
    cv::transform(canvas, any_channel, cv::Matx13f(1, 1, 1));  // nonzero if any channel set
    canvas.copyTo(frame, any_channel > 0);

    auto now = Clock::now();
    double dt = std::max(std::chrono::duration<double>(now - prev_time).count(), 1e-6);
    fps = 0.9 * fps + 0.1 * (1.0 / dt);
    prev_time = now;

    std::snprintf(buf, sizeof(buf), "FPS: %.0f   Hands: %u", fps, num_hands);
    put_label(frame, buf, {10, 30}, 0.7);
    put_label(frame,
              std::string("Draw [d]: ") + (drawing_mode ? "ON" : "OFF") +
                  "   Clear [c]   Skeleton [l]   Quit [q]",
              {10, h - 15}, 0.55);

    cv::imshow(window, frame);
    int key = cv::waitKey(1) & 0xFF;
    if (key == 'q' || key == 27) break;
    if (key == 'd') {
      drawing_mode = !drawing_mode;
      prev_draw_point.reset();
    } else if (key == 'c') {
      canvas.setTo(cv::Scalar::all(0));
    } else if (key == 'l') {
      show_skeleton = !show_skeleton;
    }
    if (cv::getWindowProperty(window, cv::WND_PROP_VISIBLE) < 1) break;
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}

int main(int argc, char** argv) {
  // Model is looked up next to the executable's source dir by default;
  // pass a path as the first argument to override.
  fs::path model_path = argc > 1 ? fs::path(argv[1]) : fs::path(MODEL_DIR) / "hand_landmarker.task";
  try {
    return run(model_path);
  } catch (const std::exception& e) {
    std::fprintf(stderr, "Error: %s\n", e.what());
    return 1;
  }
}
