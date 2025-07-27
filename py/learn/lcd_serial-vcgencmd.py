import serial
import subprocess

def run_vcgencmd(cmd):
    full_cmd = ["vcgencmd"] + cmd.split()
    result = subprocess.run(full_cmd, stdout=subprocess.PIPE, text=True)
    return result.stdout.strip()

if __name__ == '__main__':
    try:
        port = '/dev/ttyUSB0'
        baud = 115200
        serial_esp32 = serial.Serial(port, baud, timeout=1)
    except Exception:
        print("err connecting to the esp32")
        sys.exit(1)
    output = run_vcgencmd("measure_temp")
    message = "1." + output + '\n'
    send = message.encode('utf-8')
    serial_esp32.write(send)
    print(output)
