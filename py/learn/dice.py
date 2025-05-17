import random
from cust_col import print_color

def custom_gen():
    dice_list = []
    try:
        dice_int = int(input("number of dice to roll: "))
        if dice_int > 100:
            print_color("lower it", "red")
            return 
        for n in range(dice_int):
            dice_size = int(input(f"number of sides for dice {n + 1}: "))
            if dice_size > 1000:
                print_color("lower it", "red")
                return
            dice_result = random.randint(1, dice_size)
            dice_list.append({"dice" : n + 1, "side" : dice_result})
        print("dices result:")
        for x in dice_list:
            print_color(f"dice {x['dice']}: {x['side']}", "green")
    except ValueError:
        print_color("invalid number", "red")
        return 
    
def normal_gen():
    dice_list = []
    try:
        dice_int = int(input("number of dice to roll: "))
        if dice_int > 1000:
            print_color("lower it", "red")
            return 
        dice_size = int(input("number of sides per dice: "))
        if dice_size > 1000:
            print_color("lower it", "red")
            return 
        for n in range(dice_int):
            dice_output = random.randint(1, dice_size)
            dice_list.append(dice_output)
        print("dices result:")
        for i, item in enumerate(dice_list, start=1):
            print_color(f"dice {i}: {item}", "green")
    except ValueError:
        print_color("invalid number", "red")
        return

while True:
    custom = input("customize sides per dice: <y|n> ").lower().strip()
    if custom != "y" and custom != "n":
        print("invalid input")
        continue
    if custom == "y":
        custom_gen()
    else:
        normal_gen()
    quit_input = input("quit? <y> ").strip().lower()
    if quit_input == "y":
        break

