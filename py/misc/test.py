import sys

def main():
    if len(sys.argv) != 4:
        print("Usage: python calc.py <num1> <operator> <num2>")
        return

    try:
        num1 = float(sys.argv[1])
        operator = sys.argv[2]
        num2 = float(sys.argv[3])
    except ValueError:
        print("Error: Both operands must be numbers.")
        return

    if operator == '+':
        result = num1 + num2
    elif operator == '-':
        result = num1 - num2
    elif operator == '*':
        result = num1 * num2
    elif operator == '/':
        if num2 == 0:
            print("Error: Division by zero.")
            return
        result = num1 / num2
    else:
        print("Error: Unsupported operator. Use +, -, *, or /.")
        return

    print(f"The result is: {result}")

if __name__ == "__main__":
    main()

