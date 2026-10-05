// Minimal declarations for the MediaPipe Tasks C API (libmediapipe.so).
//
// libmediapipe.so ships inside the `mediapipe` pip package
// (site-packages/mediapipe/tasks/c/libmediapipe.so) but without headers.
// These layouts mirror the ctypes structs used by the Python bindings in
// site-packages/mediapipe/tasks/python/ — keep them in sync if you upgrade.

#ifndef FINGER_TRACKER_MEDIAPIPE_C_H_
#define FINGER_TRACKER_MEDIAPIPE_C_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int MpStatus;  // 0 == MP_OK
enum { MP_OK = 0 };

typedef void* MpImagePtr;
typedef void* MpHandLandmarkerPtr;

enum MpImageFormat { kMpImageFormatSrgb = 1 };
enum MpRunningMode { kMpRunningModeImage = 1, kMpRunningModeVideo = 2, kMpRunningModeLiveStream = 3 };
enum { kMpHostSystemLinux = 1 };

struct MpBaseOptions {
  const char* model_asset_buffer;
  unsigned int model_asset_buffer_count;
  const char* model_asset_path;
  int file_descriptor;
  int delegate;
  int host_environment;
  int host_system;
  const char* host_version;
  const char* ca_bundle_path;
  const char* app_id;
  const char* app_version;
};

struct MpRectF {
  float left, top, bottom, right;
};

struct MpImageProcessingOptions {
  bool has_region_of_interest;
  struct MpRectF region_of_interest;
  int rotation_degrees;
};

struct MpCategory {
  int index;
  float score;
  const char* category_name;
  const char* display_name;
};

struct MpCategories {
  struct MpCategory* categories;
  uint32_t categories_count;
};

struct MpLandmark {
  float x, y, z;
  bool has_visibility;
  float visibility;
  bool has_presence;
  float presence;
  const char* name;
};

struct MpLandmarks {
  struct MpLandmark* landmarks;
  uint32_t landmarks_count;
};

typedef struct MpLandmark MpNormalizedLandmark;

struct MpNormalizedLandmarks {
  MpNormalizedLandmark* landmarks;
  uint32_t landmarks_count;
};

struct MpHandLandmarkerResult {
  struct MpCategories* handedness;
  uint32_t handedness_count;
  struct MpNormalizedLandmarks* hand_landmarks;
  uint32_t hand_landmarks_count;
  struct MpLandmarks* hand_world_landmarks;
  uint32_t hand_world_landmarks_count;
};

typedef void (*MpHandLandmarkerResultCallback)(MpStatus status,
                                               struct MpHandLandmarkerResult* result,
                                               MpImagePtr image,
                                               int64_t timestamp_ms);

struct MpHandLandmarkerOptions {
  struct MpBaseOptions base_options;
  int running_mode;
  int num_hands;
  float min_hand_detection_confidence;
  float min_hand_presence_confidence;
  float min_tracking_confidence;
  MpHandLandmarkerResultCallback result_callback;
};

void MpErrorFree(char* error_msg);

MpStatus MpImageCreateFromUint8Data(int format, int width, int height,
                                    const uint8_t* data, int data_size,
                                    MpImagePtr* out, char** error_msg);
void MpImageFree(MpImagePtr image);

MpStatus MpHandLandmarkerCreate(struct MpHandLandmarkerOptions* options,
                                MpHandLandmarkerPtr* out, char** error_msg);
MpStatus MpHandLandmarkerDetectForVideo(MpHandLandmarkerPtr landmarker,
                                        MpImagePtr image,
                                        struct MpImageProcessingOptions* options,
                                        int64_t timestamp_ms,
                                        struct MpHandLandmarkerResult* result,
                                        char** error_msg);
void MpHandLandmarkerCloseResult(struct MpHandLandmarkerResult* result);
MpStatus MpHandLandmarkerClose(MpHandLandmarkerPtr landmarker, char** error_msg);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // FINGER_TRACKER_MEDIAPIPE_C_H_
