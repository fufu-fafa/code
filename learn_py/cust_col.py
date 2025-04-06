def print_color(text, color="reset"):
    colors = {
        "reset": "\033[0m",
        "red": "\033[91m",
        "green": "\033[92m",
        "yellow": "\033[93m",
        "blue": "\033[94m",
    }
    print(f"{colors[color]}{text}{colors['reset']}")

if __name__ == '__main__':
    valid_col = ["red", "green", "blue", "yellow"]
    text = input("input the text to print: ")
    col = input("input the color of the text output: ").strip().lower()
    if col not in valid_col:
        print_color("invalid color choice", "red")
        print(f"valid colors: {valid_col}")
        exit()
    print_color(text, col)
