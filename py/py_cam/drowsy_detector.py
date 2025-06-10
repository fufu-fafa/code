import cv2
import dlib
import serial
from scipy.spatial import distance
from imutils import face_utils

# config
mirror = True
use_lcd = False
verbose_eye = True
verbose_mouth = True
file_location = './shape_predictor_68_face_landmarks.dat'

# esp32 communication port
if use_lcd:
    port = '/dev/tty.usbserial-0001'
    baud = 115200
    serial_esp32 = serial.Serial(port, baud, timeout=1)

def line(number):
    return number * 30

def send_message(is_eye, message, state):
    if message == state:
        return state
    if is_eye:
        # prefix '1.' changing line 1
        if message == 1:
            output = "1.awake eyes\n"
        elif message == 2:
            output = "1.possibly drowsy\n"
        else:
            output = "1.DROWSY EYES\n"
    else:
        # prefix '2.' changing line 2
        if message == 1:
            output = "2.normal mouth\n"
        elif message == 2:
            output = "2.possible yawning\n"
        else:
            output = "2.YAWNING\n"
    send = output.encode('utf-8')
    serial_esp32.write(send)
    state = message
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

EAR_THRESHOLD = 0.25
CONSEC_FRAMES = 36

MAR_THRESHOLD = 0.3
YAWN_FRAMES = 10

counter = 0
yawn_counter = 0
ear = 0
mar = 0
mouth_state = 0
eye_state = 0

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(file_location)

(lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]
(mStart, mEnd) = face_utils.FACIAL_LANDMARKS_IDXS["mouth"]

cap = cv2.VideoCapture(0)

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
        leftEAR = eye_aspect_ratio(leftEye)
        rightEAR = eye_aspect_ratio(rightEye)
        ear = (leftEAR + rightEAR) / 2.0

        # Mouth
        mouth = shape[mStart:mEnd]
        mar = mouth_aspect_ratio(mouth)

        if verbose_eye:
            leftHull = cv2.convexHull(leftEye)
            rightHull = cv2.convexHull(rightEye)
            cv2.drawContours(frame, [leftHull], -1, (0, 255, 0), 1)
            cv2.drawContours(frame, [rightHull], -1, (0, 255, 0), 1)
            cv2.putText(frame, f"eye aspect ratio: {ear:.2f}", (10, line(3)),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

            if (ear < EAR_THRESHOLD) and (counter >= CONSEC_FRAMES):
                if use_lcd:
                    eye_state = send_message(True, 3, eye_state)
                cv2.putText(frame, "SLEEPY!", (10, line(4)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
            if (ear < EAR_THRESHOLD) and not (counter >= CONSEC_FRAMES):
                counter += 1
                if use_lcd:
                    eye_state = send_message(True, 2, eye_state)
                cv2.putText(frame, "possibly sleepy", (10, line(4)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)

            if not (ear < EAR_THRESHOLD) and not (counter >= CONSEC_FRAMES):
                if use_lcd:
                    eye_state = send_message(True, 1, eye_state)
                counter = 0
                cv2.putText(frame, "not sleepy", (10, line(4)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

        if verbose_mouth:
            mouthHull = cv2.convexHull(mouth)
            cv2.drawContours(frame, [mouthHull], -1, (0, 255, 255), 1)
            cv2.putText(frame, f"mouth aspect ratio: {mar:.2f}", (10, line(1)),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 0, 0), 2)

            if (mar > MAR_THRESHOLD) and (yawn_counter >= YAWN_FRAMES):
                if use_lcd:
                    mouth_state = send_message(False, 3, mouth_state)
                cv2.putText(frame, "YAWNING!", (10, line(2)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
            if (mar > MAR_THRESHOLD) and not (yawn_counter >= YAWN_FRAMES):
                yawn_counter += 1
                if use_lcd:
                    mouth_state = send_message(False, 2, mouth_state)
                cv2.putText(frame, "possibly yawning", (10, line(2)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)

            if not (mar > MAR_THRESHOLD) and not (yawn_counter >= YAWN_FRAMES):
                if use_lcd:
                    mouth_state = send_message(False, 1, mouth_state)
                yawn_counter = 0
                cv2.putText(frame, "not yawning", (10, line(2)),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

    cv2.imshow("Drowsy Detector", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
