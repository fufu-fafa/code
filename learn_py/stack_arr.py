from cust_col import print_color
array_stack = []

options = ["add", "view", "pop", "exit"]
print_color(f"valid options: {options}", "green")
while True:
    choice = input("input one of the valid options: ").lower().strip()
    if choice == "add":
        add_data = input("append stack: ").strip()
        array_stack.append(add_data)
    elif choice == "view":
        print("stack: ", array_stack)
    elif choice == "pop":
        continue_pop = input(f"are you sure to remove: {array_stack[-1]} <y/n> ").strip().lower()
        if continue_pop == "y":
            top = array_stack.pop()
            print("popped element: ", top)
    elif choice == "exit":
        break
    else:
        print_color("invalid options", "red")
