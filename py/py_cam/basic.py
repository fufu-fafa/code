import cv2

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

    output = cv2.flip(frame, 1)
    cv2.imshow('Webcam Feed', output)

    # Exit loop when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
