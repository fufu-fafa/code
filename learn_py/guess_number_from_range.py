import random
from cust_col import print_color

def get_max():
    while True:
        try:
            temp_num = int(input("input the maximum range for the random number: "))
            if temp_num <= 1:
                print_color("range must be more than 1", "red")
                continue
            return temp_num
        except ValueError:
            print_color("invalid number", "red")
    
def validate_input(max_num):
    while True:
        try:
            temp_guess = int(input(f"guess a number ranging from 1 to {max_num}: "))
            if temp_guess > max_num:
                print_color(f"input must range from 1 to {max_num}", "red")
                continue
            return temp_guess
        except ValueError:
            print_color("invalid number", "red")

max_num = get_max()
tries = 0
random_num = random.randint(1, max_num)
while True:
    guess_num = validate_input(max_num)
    tries += 1
    if guess_num == random_num:
        print_color("guessed right", "green")
        print_color(f"number of tries: {tries}", "yellow")
        break
    elif guess_num < random_num:
        if abs(guess_num - random_num) <= 5:
            print_color("too low, but close", "yellow")
            continue
        print("too low")
    else:
        if abs(guess_num - random_num) <= 5:
            print_color("too high, but close", "yellow")
            continue
        print("too high")
