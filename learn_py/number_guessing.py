import random
from cust_col import print_color

while True:
    try:
        range_input = int(input("input the upper limit of the number: "))
        num_input = int(input("input a number to guess: "))
        if num_input > range_input or num_input < 1:
            print_color(f"input must range from 1-{range_input}", "red")
            continue
        num_random = random.randint(1, range_input)
        if num_input == num_random:
            print_color("lucky", "green")
        else:
            print_color("unlucky", "red")
        print(f"random number: {num_random}")
        input_exit = input("exit? <y/n> ")
        if input_exit == "y":
            break
    except ValueError:
        print_color("invalid number", "red")
        print("")
        continue
