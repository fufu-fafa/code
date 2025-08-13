import sys
import json
import time
import subprocess
from datetime import datetime

CLI_PATH = "/opt/homebrew/bin/CoreLocationCLI"

while True:
    try:
        result = subprocess.run(
            [CLI_PATH],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        if result.returncode != 0:
            print(f"[ERROR] {result.stderr.strip()}")
        else:
            lat_str, lon_str = result.stdout.strip().split(" ")

            location_data = {
                "latitude": float(lat_str),
                "longitude": float(lon_str)
            }

            # Save to file
            with open("location.json", "w") as f:
                json.dump(location_data, f, indent=2)

            print(f"[OK] Location saved: {location_data}")
            time.sleep(30)

    except KeyboardInterrupt:
        print("exiting..")
        sys.exit(0)
    except Exception as e:
        print(f"[EXCEPTION] {e}")
