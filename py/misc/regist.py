import requests
import random
import string
import time
from concurrent.futures import ThreadPoolExecutor

url = "http://localhost:8203/register"
delay = 0.001
passlist = []
length = 12
logfile = "registered_usr.log"
threads = 2

def import_pass(filename, passlist):
    with open(filename, "r") as file:
        for line in file:
            passlist.append(line.strip())

def rand_user(length):
    return ''.join(random.choices(string.ascii_lowercase + string.digits, k=length))

def generate_users():
    return {"username": rand_user(length), "password": random.choice(passlist)}

def register_user(_):
    user = generate_users()
    try:
        response = requests.post(url, json=user)
        print(f"status: {response.status_code} | user: {user}")
        with open(logfile, "a") as log:
            log.write(f"{user}\n")
        time.sleep(delay)
    except Exception as e:
        print(f"error: {e}")

if __name__ == '__main__':
    import_pass("rockyou_clean.txt", passlist)
    try:
        input_int = int(input("how many users: ").strip())
        with ThreadPoolExecutor(max_workers=threads) as executor:
            executor.map(register_user, range(input_int))
    except ValueError:
        print("invalid number")
