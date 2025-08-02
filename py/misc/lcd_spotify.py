import sys
import time
import serial
import subprocess

last_msg = ""
delay = 8
cmd = [
    'osascript',
    '-e',
    'tell application "Spotify" to if it is running and player state is playing then name of current track & " - " & artist of current track'
]

try:
    port = '/dev/tty.usbserial-0001'
    baud = 115200
    serial_esp32 = serial.Serial(port, baud, timeout=1)
except Exception:
    print("err connecting to the esp32")
    sys.exit(1)

try:
    while True:
        time.sleep(delay)
        result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        info = "2.|" + result.stdout.strip() + "|\n"
        if info == last_msg:
            continue
        print(info)
        send = info.encode('utf-8')
        serial_esp32.write(send)
        last_msg = info
except KeyboardInterrupt:
    print("exiting")
    serial_esp32.close()
    sys.exit(0)
except Exception as e:
    print(f"err: {e}")
    serial_esp32.close()
    sys.exit(0)
