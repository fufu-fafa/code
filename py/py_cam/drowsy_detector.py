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
verbose_eye = True
verbose_mouth = True

file_location = './shape_predictor_68_face_landmarks.dat'
EAR_THRESHOLD = 0.25
EYE_FRAMES = 32
MAR_THRESHOLD = 0.3
YAWN_FRAMES = 10

ear = 0
eye_counter = 0
eye_state = 0
prev_eye = 0
mar = 0
mouth_counter = 0
mouth_state = 0
prev_mouth = 0

# esp32 communication port
if use_lcd:
    try:
        port = '/dev/tty.usbserial-0001'    # if mac tty.usbserial-0001
        baud = 115200                       # if linux ttyUSB0
        serial_esp32 = serial.Serial(port, baud, timeout=1)
    except Exception:
        print("\nerror using esp32 communication port")
        sys.exit(1)

    if not verbose_eye:
        serial_esp32.write(b"1.\n")
    if not verbose_mouth:
        serial_esp32.write(b"2.\n")

def line(number: int):
    return number * 30

def send_message(is_eye: bool, message: int):
    if is_eye:
        # prefix '1.' changing line 1
        if message == 1:
            output = "1.awake eyes\n"
        elif message == 2:
            output = "1.possibly drowsy\n"
        elif message == 3:
            output = "1.DROWSY EYES\n"
        else:
            output = "1.\n"
    else:
        # prefix '2.' changing line 2
        if message == 1:
            output = "2.normal mouth\n"
        elif message == 2:
            output = "2.possible yawning\n"
        elif message == 3:
            output = "2.YAWNING\n"
        else:
            output = "1.\n"
    send = output.encode('utf-8')
    serial_esp32.write(send)

def lcd_handler(is_eye: bool, prev_state: int, state: int):
    if prev_state == state:
        return prev_state
    send_message(is_eye, state)
    return state

def eye_aspect_ratio(eye):
    A = distance.euclidean(eye[1], eye[5])
    B = distance.euclidean(eye[2], eye[4])
    C = distance.euclidean(eye[0], eye[3])
    return (A + B) / (2.0 * C)

def mouth_aspect_ratio(mouth):
    A = distance.euclidean(mouth[13], mouth[19])
    B = distance.euclidean(mouth[14], mouth[18])
    C = distance.euclidean(mouth[15], mouth[17])
    D = distance.euclidean(mouth[12], mouth[16])
    return (A + B + C) / (3.0 * D)

def draw_eyes(eyes, frame, ear, eye_state: int):
    leftHull = cv2.convexHull(eyes[0])
    rightHull = cv2.convexHull(eyes[1])
    cv2.drawContours(frame, [leftHull], -1, (0, 255, 0), 1)
    cv2.drawContours(frame, [rightHull], -1, (0, 255, 0), 1)
    cv2.putText(frame, f"eye aspect ratio: {ear:.2f}", (10, line(3)),
        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

    if eye_state == 3:
        cv2.putText(frame, "SLEEPY!", (10, line(4)),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
    elif eye_state == 2:
        cv2.putText(frame, "possibly sleepy", (10, line(4)),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)
    else:
        cv2.putText(frame, "not sleepy", (10, line(4)),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

def draw_mouth(mouth, frame, mar, mouth_state: int):
        mouthHull = cv2.convexHull(mouth)
        cv2.drawContours(frame, [mouthHull], -1, (0, 255, 255), 1)
        cv2.putText(frame, f"mouth aspect ratio: {mar:.2f}", (10, line(1)),
            cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

        if mouth_state == 3:
            cv2.putText(frame, "YAWNING!", (10, line(2)),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        elif mouth_state == 2:
            cv2.putText(frame, "possibly yawning", (10, line(2)),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)
        else:
            cv2.putText(frame, "not yawning", (10, line(2)),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

# check for common errors
try:
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor(file_location)

    (lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
    (rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]
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

        # Eyes
        leftEye = shape[lStart:lEnd]
        rightEye = shape[rStart:rEnd]
        eyes = [leftEye, rightEye]
        leftEAR = eye_aspect_ratio(leftEye)
        rightEAR = eye_aspect_ratio(rightEye)
        ear = (leftEAR + rightEAR) / 2.0
        eye_condition = [ear < EAR_THRESHOLD, eye_counter >= EYE_FRAMES]

        # Mouth
        mouth = shape[mStart:mEnd]
        mar = mouth_aspect_ratio(mouth)
        mouth_condition = [mar > MAR_THRESHOLD, mouth_counter >= YAWN_FRAMES]

        if eye_condition[0] and eye_condition[1]:
            eye_state = 3
        elif eye_condition[0]:
            eye_counter += 1
            eye_state = 2
        else:
            eye_counter = 0
            eye_state = 1

        if mouth_condition[0] and mouth_condition[1]:
            mouth_state = 3
        elif mouth_condition[0]:
            mouth_counter += 1
            mouth_state = 2
        else:
            mouth_counter = 0
            mouth_state = 1

        if use_lcd:
            prev_eye = lcd_handler(True, prev_eye, eye_state)
            prev_mouth = lcd_handler(False, prev_mouth, mouth_state)

        if verbose_eye:
            draw_eyes(eyes, frame, ear, eye_state)

        if verbose_mouth:
            draw_mouth(mouth, frame, mar, mouth_state)

    cv2.imshow("Drowsy Detector", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        if use_lcd:
            send_message(0, 0)
            send_message(1, 0)
            serial_esp32.close()
        break

cap.release()
cv2.destroyAllWindows()
