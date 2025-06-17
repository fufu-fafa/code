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
use_lcd = False
verbose_mouth = True

file_location = './shape_predictor_68_face_landmarks.dat'
MAR_THRESHOLD = 0.3
YAWN_FRAMES = 10

counter = 0
yawn_counter = 0
ear = 0
mar = 0
eye_state = 0
mouth_state = 0

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

def send_message(message: int, state: int):
    if message == state:
        return state
    if message == 1:
        output = "1.normal mouth\n"
    elif message == 2:
        output = "1.possible yawning\n"
    else:
        output = "1.YAWNING\n"
    send = output.encode('utf-8')
    serial_esp32.write(send)
    state = message
    return state

def mouth_aspect_ratio(mouth):
    A = distance.euclidean(mouth[13], mouth[19])
    B = distance.euclidean(mouth[14], mouth[18])
    C = distance.euclidean(mouth[15], mouth[17])
    D = distance.euclidean(mouth[12], mouth[16])
    return (A + B + C) / (3.0 * D)

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
    for face in faces:
        shape = predictor(gray, face)
        shape = face_utils.shape_to_np(shape)

        # Mouth
        mouth = shape[mStart:mEnd]
        mar = mouth_aspect_ratio(mouth)

        if verbose_mouth:
            mouthHull = cv2.convexHull(mouth)
            cv2.drawContours(frame, [mouthHull], -1, (0, 255, 255), 1)
            cv2.putText(frame, f"mouth aspect ratio: {mar:.2f}", (10, line(1)),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

            if (mar > MAR_THRESHOLD) and (yawn_counter >= YAWN_FRAMES):
                if use_lcd:
                    mouth_state = send_message(3, mouth_state)
                cv2.putText(frame, "YAWNING!", (10, line(2)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
            elif (mar > MAR_THRESHOLD):
                yawn_counter += 1
                if use_lcd:
                    mouth_state = send_message(2, mouth_state)
                cv2.putText(frame, "possibly yawning", (10, line(2)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)

            else:
                if use_lcd:
                    mouth_state = send_message(1, mouth_state)
                yawn_counter = 0
                cv2.putText(frame, "not yawning", (10, line(2)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

    cv2.imshow("Drowsy Detector", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
