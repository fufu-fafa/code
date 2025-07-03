import sys
import cv2
import dlib
import serial
from scipy.spatial import distance
from imutils import face_utils

# '0' for laptop camera, anything else for external
camera_used = 0

# config
mirror = True
use_lcd = True
verbose_mouth = True

file_location = './shape_predictor_68_face_landmarks.dat'
MAR_THRESHOLD = 0.3
YAWN_FRAMES = 10

is_face_detected = 0
mar = 0
mouth_state = 0
yawn_counter = 0

# esp32 communication port
if use_lcd:
    try:
        port = '/dev/tty.usbserial-0001'
        baud = 115200
        serial_esp32 = serial.Serial(port, baud, timeout=1)
    except Exception:
        print("\nerror using esp32 communication port")
        sys.exit(1)

def line(number: int):
    return number * 30

def display_mouth_state(message: int, state: int):
    if message == state:
        return state
    if message == 1:
        output = "1.normal mouth\n"
    elif message == 2:
        output = "1.possible yawning\n"
    elif message == 3:
        output = "1.YAWNING\n"
    else:
        output = "1."
    send = output.encode('utf-8')
    serial_esp32.write(send)
    return message

def mouth_aspect_ratio(mouth):
    A = distance.euclidean(mouth[13], mouth[19])
    B = distance.euclidean(mouth[14], mouth[18])
    C = distance.euclidean(mouth[15], mouth[17])
    D = distance.euclidean(mouth[12], mouth[16])
    return (A + B + C) / (3.0 * D)

def draw_mouth(frame, mouth, mar, conditions):
    mouthHull = cv2.convexHull(mouth)
    cv2.drawContours(frame, [mouthHull], -1, (0, 255, 255), 1)
    cv2.putText(frame, f"mouth aspect ratio: {mar:.2f}", (10, line(1)),
        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

    if conditions[0] and conditions[1]:
        cv2.putText(frame, "YAWNING!", (10, line(2)),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
    elif conditions[0]:
        cv2.putText(frame, "possibly yawning", (10, line(2)),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)

    else:
        cv2.putText(frame, "not yawning", (10, line(2)),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

# check for common errors
try:
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor(file_location)
    (mStart, mEnd) = face_utils.FACIAL_LANDMARKS_IDXS["mouth"]

    cap = cv2.VideoCapture(camera_used)
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
except Exception as e:
    if "landmarks.dat" in str(e):
        print("\nunable to open facial detector model, check file location")
    else:
        print("\nfailed to initialize, check config and imports")
    sys.exit(1)

while True:
    ret, frame = cap.read()
    if mirror:
        frame = cv2.flip(frame, 1)

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = detector(gray)

    if len(faces) > 0:
        shape = predictor(gray, faces[0])
        shape = face_utils.shape_to_np(shape)

        mouth = shape[mStart:mEnd]
        mar = mouth_aspect_ratio(mouth)
        conditions = [mar > MAR_THRESHOLD, yawn_counter >= YAWN_FRAMES]

        if conditions[0] and conditions[1]:
            if use_lcd:
                mouth_state = display_mouth_state(3, mouth_state)
        elif conditions[0]:
            yawn_counter += 1
            if use_lcd:
                mouth_state = display_mouth_state(2, mouth_state)
        else:
            yawn_counter = 0
            if use_lcd:
                mouth_state = display_mouth_state(1, mouth_state)

        if verbose_mouth:
            draw_mouth(frame, mouth, mar, conditions)

    else:
        if use_lcd:
            mouth_state = display_mouth_state(0, mouth_state)

    cv2.imshow("Drowsy Detector", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        if use_lcd:
            serial_esp32.close()
        break

cap.release()
cv2.destroyAllWindows()
