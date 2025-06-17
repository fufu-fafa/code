import serial
import sys

try:
    port = '/dev/tty.usbserial-0001'
    baud = 115200
    serial_esp32 = serial.Serial(port, baud, timeout=1)
except Exception:
    print("err connecting to the esp32")
    sys.exit(1)

try:
    while True:
        message = input("send message to esp32: ").strip()
        if message:
            send = message.encode('utf-8')
            serial_esp32.write(send)
except KeyboardInterrupt:
    print("\nexiting..")
    serial_esp32.close()
    sys.exit(0)
