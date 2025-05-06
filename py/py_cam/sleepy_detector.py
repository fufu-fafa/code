import cv2
import dlib
from scipy.spatial import distance
from imutils import face_utils

def eye_aspect_ratio(eye):
    A = distance.euclidean(eye[1], eye[5])
    B = distance.euclidean(eye[2], eye[4])
    C = distance.euclidean(eye[0], eye[3])
    return (A + B) / (2.0 * C)

EAR_THRESHOLD = 0.25
CONSEC_FRAMES = 48

counter = 0

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")

(lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = detector(gray)
    for face in faces:
        shape = predictor(gray, face)
        shape = face_utils.shape_to_np(shape)

        leftEye = shape[lStart:lEnd]
        rightEye = shape[rStart:rEnd]

        leftEAR = eye_aspect_ratio(leftEye)
        rightEAR = eye_aspect_ratio(rightEye)

        ear = (leftEAR + rightEAR) / 2.0

        if ear < EAR_THRESHOLD:
            counter += 1
            cv2.putText(frame, "possibly sleepy", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)
            if counter >= CONSEC_FRAMES:
                cv2.putText(frame, "SLEEPY!", (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        else:
            counter = 0
            cv2.putText(frame, "not sleepy", (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)


        # Draw eyes (optional)
        leftHull = cv2.convexHull(leftEye)
        rightHull = cv2.convexHull(rightEye)
        cv2.drawContours(frame, [leftHull], -1, (0, 255, 0), 1)
        cv2.drawContours(frame, [rightHull], -1, (0, 255, 0), 1)

    cv2.imshow("Sleepy Detector", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

