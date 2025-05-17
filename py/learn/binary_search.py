from cust_col import print_color

def guess_subtract(low, mid):
    return low, mid - 1

def guess_add(mid, high):
    return mid + 1, high

def validate_input():
    while True:
        try:
            temp = int(input("input '1' if too high, '2' if too low, '3' to exit: "))
            if temp not in (1, 2, 3):
                print("input '1', '2', or '3'")
                continue
            return temp
        except ValueError:
            print_color("invalid number")

low = 1
try:
    high = int(input("the maximum value of the random number: "))
    if high <= low:
        print_color("maximum must be greater than 1", "red")
    else:
        while True:
            mid = (low + high) // 2
            print_color(f"guess {mid}", "yellow")
            in_guess = validate_input()
            if in_guess == 3:
                print("exiting")
                break
            elif in_guess == 1:
                low, high = guess_subtract(low, mid)
            else:
                low, high = guess_add(mid, high)
except ValueError:
    print_color("invalid number", "red")