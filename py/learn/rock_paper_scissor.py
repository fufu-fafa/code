import random
from cust_col import print_color

win_condition = {
    "rock": "scissor",
    "paper": "rock",
    "scissor": "paper"
}
valid = {1: "rock", 2: "paper", 3: "scissor"}
print("choices:")
for x, y in valid.items():
    print(f"{x}. {y}")
while True:
    try:
        temp_choice = int(input("choose one: "))
        if temp_choice not in valid:
            print_color("invalid choice", "red")
            continue
        choice = valid[temp_choice]
        computer = valid[random.randint(1, 3)]
        print(f"computer: {computer}")
        if choice == computer:
            print("draw")
        elif win_condition[choice] == computer:
            print_color("won", "green")
        else:
            print_color("lost", "red")
        exit = input("exit? <y/n> ").lower().strip()
        if exit == "y":
            break

    except ValueError:
        print_color("input a number", "red")