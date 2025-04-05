import sys
import random

def print_color(text, color="reset"):
    colors = {
        "reset": "\033[0m",
        "red": "\033[91m",
        "green": "\033[92m",
        "yellow": "\033[93m",
        "blue": "\033[94m",
    }
    print(f"{colors[color]}{text}{colors['reset']}")

def checker(x, y, rand_x, rand_y):
    if x == rand_x and y == rand_y:
        print_color(f"chosen and random: ({x}, {y}) ({rand_x}, {rand_y})", "green")
        print_color("0.01% chance of getting this", "red")
    elif x == rand_x or y == rand_y:
        print_color(f"chosen and random: ({x}, {y}) ({rand_x}, {rand_y})", "green")
        print_color("1.99% chance of getting this", "yellow")
    else:
        print_color(f"chosen and random: ({x}, {y}) ({rand_x}, {rand_y})", "green")
        print_color("98% chance of getting this", "green")

rand_x = random.randint(1, 100)
rand_y = random.randint(1, 100)

if len(sys.argv) != 3:
    print_color(f"usage: {sys.argv[0]} <first cord> <second cord>", "red")
    sys.exit(1)
else:
    try:
        x = int(sys.argv[1])
        y = int(sys.argv[2])
        if x > 100 or x < 1 or y > 100 or y < 1:
            print_color("input a number from 1-100", "red")
            sys.exit(1)
        else:
            checker(x, y, rand_x, rand_y)
    except ValueError: 
        print_color("input invalid", "red")
        sys.exit(1)

   
