def print_color(text, color="reset"):
    colors = {
        "reset": "\033[0m",
        "red": "\033[91m",
        "green": "\033[92m",
        "yellow": "\033[93m",
        "blue": "\033[94m",
    }
    print(f"{colors[color]}{text}{colors['reset']}")

col_used = input("input the color to use: ").strip().lower()
if col_used not in ("red", "green", "yellow", "blue"):
    print_color("invalid color", "red")
    exit()
while True:
    msgs = input("the message to output, ' ' to exit: ")
    print_color(msgs, col_used)
    if msgs == " ":
        break
