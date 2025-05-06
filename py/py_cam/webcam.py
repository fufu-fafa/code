import cv2

# Load pre-trained Haar Cascade for face detection
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Open the default webcam (device 0)
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Cannot open webcam")
    exit()

while True:
    ret, frame = cap.read()

    if not ret:
        print("Error: Can't receive frame (stream end?). Exiting...")
        break

    mirrored = cv2.flip(frame, 1)

    gray = cv2.cvtColor(mirrored, cv2.COLOR_BGR2GRAY)  # Convert to grayscale

    # Detect faces
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5)

    # Draw rectangles around detected faces
    for (x, y, w, h) in faces:
        cv2.rectangle(mirrored, (x, y), (x + w, y + h), (0, 255, 0), 2)

    cv2.imshow('Webcam Feed (Face Detection)', mirrored)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

