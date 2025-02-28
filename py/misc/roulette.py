import random
import sys

colors = {"black", "green", "red"}
col_black = {2, 4, 6, 8, 10, 11, 13, 15, 17, 20, 22, 24, 26, 28, 29, 31, 33, 35}
roulette = random.randint(0,36)

def print_color(text, color="reset"):
    colors = {
        "reset": "\033[0m",
        "red": "\033[91m",
        "green": "\033[92m",
        "yellow": "\033[93m",
        "blue": "\033[94m",
    }
    print(f"{colors[color]}{text}{colors['reset']}")

def roulette_color(roulette):
    if roulette == 0:
        return "green"
    elif roulette in col_black:
        return "black"
    else:
        return "red"

def checker(roulette, col_roulette, col_pick, num_pick):
    if num_pick == roulette and col_pick == col_roulette:
        print_color("very lucky", "green")
    elif num_pick == roulette or col_pick == col_roulette:
        print_color("lucky", "yellow")
    else:
        print_color("unlucky", "red")

def main():
    try:
        col_roulette = roulette_color(roulette)
        num_pick = int(input("input a valid number: "))
        if num_pick > 36 or num_pick < 0:
            print_color("invalid number input, valid number ranges from 0-36", "red")
            sys.exit(1)
        col_pick = input("input a valid color: ").strip().lower()
        if col_pick not in colors:
            print_color("invalid color input, input 'red', 'black', or 'green'", "red")
            sys.exit(1)
        checker(roulette, col_roulette, col_pick, num_pick)
        print_color(f"roulette result: {roulette} {col_roulette}", "yellow")
        
    except ValueError:
        print_color("invalid number input, input ranges from 0-36", "red")
        sys.exit(1)

if __name__ == "__main__":
    main()
