import re
import time
import serial
import subprocess

cmd = ["sudo", "dd", "if=/dev/mmcblk0", "of=/mnt/usb/pi5_backup.img", "bs=4M", "status=progress", "conv=fsync"]

try:
    port = '/dev/ttyUSB0'
    baud = 115200
    serial_esp32 = serial.Serial(port, baud, timeout=1)
except Exception:
    print("err connecting to the esp32")
    sys.exit(1)

# Regex pattern to capture progress output
progress_pattern = re.compile(r'([\d.]+)\s+([KMG]B),\s+([\d.]+)\s+([KMG]iB)')

last_print_time = 0
interval = 5

with subprocess.Popen(cmd, stderr=subprocess.PIPE, text=True) as proc:
    for line in proc.stderr:
        now = time.time()
        line = line.strip()
        match = progress_pattern.search(line)
        if match and (now - last_print_time) >= interval:
            amount_1, unit_1, amount_2, unit_2 = match.groups()
            output = f"1.Copied: {amount_1} {unit_1} / {amount_2} {unit_2}\n"
            send = output.encode('utf-8')
            serial_esp32.write(send)
            last_print_time = now
