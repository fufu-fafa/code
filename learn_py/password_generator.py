import random

def file_exist(output_name):
    try:
        with open(output_name, "r") as file:
            return True
    except FileNotFoundError:
        return False

def write_output(password):
    output_name = input("save the password in file named: ")
    if file_exist(output_name):
        bypass = input("file already exist, overwrite? <y/n> ").lower().strip()
        if bypass != "y":
            print("quitting..")
            return
    with open(output_name, "w") as file:
        file.write(password)
    print(f"file saved in {output_name}")

symbols = list("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~")

while True:
    try:
        range_input = int(input("input the number of character in password: "))
        if range_input <= 8 or range_input > 256:
            print("input must range from 8-256")
            continue
        password = "".join(random.choices(symbols, k=range_input))
        save = input("save as file? <y/n> ").strip().lower()
        if save == "y":
            write_output(password)
        print(password)
        break
    except ValueError:
        print("invalid number")
