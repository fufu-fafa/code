"""Real-time finger tracker using MediaPipe's HandLandmarker model.

Controls:
    q / Esc  quit
    d        toggle air-drawing with the index fingertip
    c        clear the drawing
    l        toggle landmark skeleton
"""

import time
from collections import deque
from pathlib import Path

import cv2
import mediapipe as mp
import numpy as np
from mediapipe.tasks.python import BaseOptions, vision

MODEL_PATH = Path(__file__).parent / "hand_landmarker.task"

# Landmark indices (see MediaPipe hand landmark model docs)
WRIST = 0
THUMB_TIP, THUMB_IP, THUMB_MCP = 4, 3, 2
FINGER_TIPS = {"Index": 8, "Middle": 12, "Ring": 16, "Pinky": 20}
FINGER_PIPS = {"Index": 6, "Middle": 10, "Ring": 14, "Pinky": 18}

HAND_CONNECTIONS = [
    (c.start, c.end) for c in vision.HandLandmarksConnections.HAND_CONNECTIONS
]

TIP_COLORS = {  # BGR
    THUMB_TIP: (255, 128, 0),
    8: (0, 255, 0),
    12: (0, 255, 255),
    16: (255, 0, 255),
    20: (0, 128, 255),
}


def create_landmarker(num_hands: int = 2) -> vision.HandLandmarker:
    options = vision.HandLandmarkerOptions(
        base_options=BaseOptions(model_asset_path=str(MODEL_PATH)),
        running_mode=vision.RunningMode.VIDEO,
        num_hands=num_hands,
        min_hand_detection_confidence=0.5,
        min_hand_presence_confidence=0.5,
        min_tracking_confidence=0.5,
    )
    return vision.HandLandmarker.create_from_options(options)


def to_pixels(landmarks, width: int, height: int) -> list[tuple[int, int]]:
    return [(int(lm.x * width), int(lm.y * height)) for lm in landmarks]


def raised_fingers(landmarks, handedness: str) -> list[str]:
    """Return names of fingers that are extended.

    The frame is mirrored before detection, so MediaPipe's handedness label
    matches the user's real hand.
    """
    up = []
    # Thumb: compare x of tip vs IP joint; direction depends on which hand.
    tip_x, ip_x = landmarks[THUMB_TIP].x, landmarks[THUMB_IP].x
    if (handedness == "Right" and tip_x < ip_x) or (
        handedness == "Left" and tip_x > ip_x
    ):
        up.append("Thumb")
    # Other fingers: tip above PIP joint (smaller y) means extended.
    for name, tip in FINGER_TIPS.items():
        if landmarks[tip].y < landmarks[FINGER_PIPS[name]].y:
            up.append(name)
    return up


def draw_hand(frame, points, show_skeleton: bool) -> None:
    if show_skeleton:
        for a, b in HAND_CONNECTIONS:
            cv2.line(frame, points[a], points[b], (200, 200, 200), 2)
        for p in points:
            cv2.circle(frame, p, 3, (255, 255, 255), -1)
    for idx, color in TIP_COLORS.items():
        cv2.circle(frame, points[idx], 9, color, -1)
        cv2.circle(frame, points[idx], 9, (0, 0, 0), 1)


def put_label(frame, text, org, scale=0.6, color=(255, 255, 255)) -> None:
    cv2.putText(frame, text, org, cv2.FONT_HERSHEY_SIMPLEX, scale, (0, 0, 0), 4, cv2.LINE_AA)
    cv2.putText(frame, text, org, cv2.FONT_HERSHEY_SIMPLEX, scale, color, 1, cv2.LINE_AA)


def main() -> None:
    if not MODEL_PATH.exists():
        raise SystemExit(f"Model not found: {MODEL_PATH}")

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        raise SystemExit(
            "Could not open the camera. On macOS, grant camera access to your "
            "terminal in System Settings > Privacy & Security > Camera."
        )
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

    landmarker = create_landmarker()
    canvas = None
    trail = deque(maxlen=32)  # short fading trail behind the index tip
    prev_draw_point = None
    drawing_mode = False
    show_skeleton = True
    start = time.monotonic()
    last_ts = -1
    fps, prev_time = 0.0, time.monotonic()

    window = "Finger Tracker"
    cv2.namedWindow(window, cv2.WINDOW_NORMAL)

    try:
        while True:
            ok, frame = cap.read()
            if not ok:
                break
            frame = cv2.flip(frame, 1)  # mirror view
            h, w = frame.shape[:2]
            if canvas is None:
                canvas = np.zeros_like(frame)

            rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb)
            ts = int((time.monotonic() - start) * 1000)
            ts = max(ts, last_ts + 1)  # timestamps must strictly increase
            last_ts = ts
            result = landmarker.detect_for_video(mp_image, ts)

            index_tip = None
            for i, landmarks in enumerate(result.hand_landmarks):
                handedness = result.handedness[i][0].category_name
                points = to_pixels(landmarks, w, h)
                draw_hand(frame, points, show_skeleton)

                up = raised_fingers(landmarks, handedness)
                x, y = points[WRIST]
                put_label(frame, f"{handedness}: {len(up)} up", (x - 40, y + 30), 0.7, (0, 255, 255))
                put_label(frame, ", ".join(up) or "fist", (x - 40, y + 55), 0.5)

                if i == 0:
                    index_tip = points[FINGER_TIPS["Index"]]
                    put_label(frame, f"({index_tip[0]}, {index_tip[1]})",
                              (index_tip[0] + 12, index_tip[1] - 12), 0.5, (0, 255, 0))
                    # Draw only while the index finger alone (or with thumb) is raised
                    pen_down = drawing_mode and "Index" in up and "Middle" not in up
                    if pen_down:
                        if prev_draw_point is not None:
                            cv2.line(canvas, prev_draw_point, index_tip, (0, 255, 0), 6, cv2.LINE_AA)
                        prev_draw_point = index_tip
                    else:
                        prev_draw_point = None

            if index_tip is None:
                prev_draw_point = None
            trail.append(index_tip)

            # Fading trail
            for j in range(1, len(trail)):
                if trail[j - 1] is None or trail[j] is None:
                    continue
                thickness = max(1, int(8 * j / len(trail)))
                cv2.line(frame, trail[j - 1], trail[j], (0, 200, 0), thickness, cv2.LINE_AA)

            # Overlay the drawing canvas
            mask = canvas.any(axis=2)
            frame[mask] = canvas[mask]

            now = time.monotonic()
            fps = 0.9 * fps + 0.1 * (1.0 / max(now - prev_time, 1e-6))
            prev_time = now

            put_label(frame, f"FPS: {fps:.0f}   Hands: {len(result.hand_landmarks)}", (10, 30), 0.7)
            put_label(frame, f"Draw [d]: {'ON' if drawing_mode else 'OFF'}   "
                             f"Clear [c]   Skeleton [l]   Quit [q]", (10, h - 15), 0.55)

            cv2.imshow(window, frame)
            key = cv2.waitKey(1) & 0xFF
            if key in (ord("q"), 27):
                break
            if key == ord("d"):
                drawing_mode = not drawing_mode
                prev_draw_point = None
            elif key == ord("c"):
                canvas[:] = 0
            elif key == ord("l"):
                show_skeleton = not show_skeleton
            if cv2.getWindowProperty(window, cv2.WND_PROP_VISIBLE) < 1:
                break
    finally:
        landmarker.close()
        cap.release()
        cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
