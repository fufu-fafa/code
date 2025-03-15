def print_color(text, color="reset"):
    colors = {
        "reset": "\033[0m",
        "red": "\033[91m",
        "green": "\033[92m",
        "yellow": "\033[93m",
        "blue": "\033[94m",
    }
    print(f"{colors[color]}{text}{colors['reset']}")