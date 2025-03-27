from cust_col import print_color

col_used = input("input the color to use: ").strip().lower()
if col_used not in ("red", "green", "yellow", "blue"):
    print_color("invalid color", "red")
    exit()
while True:
    msgs = input("the message to output, ' ' to exit: ")
    print_color(msgs, col_used)
    if msgs == " ":
        break
