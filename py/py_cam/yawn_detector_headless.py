import sys
import cv2
import dlib
import time
import serial
from scipy.spatial import distance
from imutils import face_utils

# '0' for laptop camera, anything else for external
camera_used = 0

# config
use_lcd = True
verbose_mouth = True  # prints mouth ratio and state to console

file_location = './shape_predictor_68_face_landmarks.dat'
MAR_THRESHOLD = 0.3
YAWN_FRAMES = 10

new_state = 0
mouth_state = 0
yawn_counter = 0

# esp32 communication port
if use_lcd:
    try:
        port = '/dev/tty.usbserial-0001'        # if mac tty.usbserial-0001
        baud = 115200                           # if linux ttyUSB0
        serial_esp32 = serial.Serial(port, baud, timeout=1)
    except Exception:
        print("\nError: unable to open ESP32 serial port")
        sys.exit(1)

# compute mouth aspect ratio
def mouth_aspect_ratio(mouth):
    A = distance.euclidean(mouth[13], mouth[19])
    B = distance.euclidean(mouth[14], mouth[18])
    C = distance.euclidean(mouth[15], mouth[17])
    D = distance.euclidean(mouth[12], mouth[16])
    return (A + B + C) / (3.0 * D)

# send state to ESP32
def send_mouth_state(last_state, new_state):
    if new_state == last_state:
        return last_state
 
    elif new_state == 1:
        msg = "1.normal mouth\n"
    elif new_state == 2:
        msg = "1.possible yawning\n"
    elif new_state == 3:
        msg = "1.YAWNING\n"
    else:
        msg = "1.\n"
    if use_lcd:
        serial_esp32.write(msg.encode('utf-8'))
    return new_state

# setup dlib detector and predictor
try:
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor(file_location)
    (mStart, mEnd) = face_utils.FACIAL_LANDMARKS_IDXS['mouth']
    cap = cv2.VideoCapture(camera_used)
    if not cap.isOpened():
        raise IOError("Camera not accessible")
except Exception as e:
    print(f"Initialization error: {e}")
    sys.exit(1)

print("Headless drowsy detector started...")

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = detector(gray, 0)

        if faces:
            shape = predictor(gray, faces[0])
            shape = face_utils.shape_to_np(shape)
            mouth = shape[mStart:mEnd]
            mar = mouth_aspect_ratio(mouth)
            is_open = mar > MAR_THRESHOLD

            if is_open and yawn_counter >= YAWN_FRAMES:
                new_state = 3
            elif is_open:
                yawn_counter += 1
                new_state = 2
            else:
                yawn_counter = 0
                new_state = 1

            mouth_state = send_mouth_state(mouth_state, new_state)

            if verbose_mouth:
                print(f"MAR: {mar:.2f}, State: {mouth_state}, Counter: {yawn_counter}")
        else:
            new_state = 0
            mouth_state = send_mouth_state(mouth_state, new_state)
            if verbose_mouth:
                print("No face detected, resetting state.")

        # limit process
        time.sleep(0.01)

except KeyboardInterrupt:
    cap.release()
    if use_lcd:
        send_mouth_state(mouth_state, 0)
        serial_esp32.close()
    print("\nHeadless detector stopped.")
    sys.exit(0)
