import random
from cust_col import print_color

while True:
    num = random.randint(1,100)
    bin_num = bin(num)[2:]
    hex_num = hex(num)[2:]
    print_color(bin_num, "green")
    show_num = input("show decimal: <y/n> ").strip().lower()
    if show_num == "y":
        print(num)
    show_hex = input("show hex: <y/n> ").strip().lower()
    if show_hex == "y":
        print_color(hex_num, "yellow")
    exit_bool = input("exit: <y/n> ").strip().lower()
    if exit_bool == "y":
        print_color("exiting..", "green")
        print("")
        break
    else:
        print("")
