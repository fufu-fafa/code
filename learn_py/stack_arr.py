from cust_col import print_color

def display_options(options): 
    print_color(f"options: {options}", "green")

def add(stack):
    item = input("item to append into stack: ").strip()
    stack.append(item)

def view(stack):
    print(f"stack: {stack}")

def pop(stack):
    if not stack:
        print_color("stack is empty", "red")
        return
    confirm = input(f"are you sure to remove {stack[-1]}: <y/n> ").strip().lower()
    if confirm == "y":
        popped = stack.pop()
        print(f"popped element: {popped}")
    else:
        print_color("no changes made", "yellow")

def run_stack_man():
    stack = []
    options = ["add", "view", "pop", "exit"]
    display_options(options)
    
    while True:
        choice = input("input one of the valid options: ").strip().lower()
        if choice == "add":
            add(stack)
        elif choice == "view":
            view(stack)
        elif choice == "pop":
            pop(stack)
        elif choice == "exit":
            break
        else:
            print_color("invalid option", "red")
            display_options(options)

if __name__ == '__main__':
    run_stack_man()
