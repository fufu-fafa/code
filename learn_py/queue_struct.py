from cust_col import print_color

def display_options(options):
    print_color(f"options: {options}", "green")

def add(queue_struct):
    item = input("item to append into queue: ").strip()
    queue_struct.append(item)

def view(queue_struct):
    print_color(f"queue: {queue_struct}", "blue")

def peek(queue_struct):
    print_color(f"front: {queue_struct[0]}")

def pop(queue_struct):
    confirm = input("are you sure to remove {queue_struct[0]}: <y/n> ").strip().lower()
    if confirm == "y":
        popped = queue_struct.pop()
        print_color(f"popped element: {popped}", "red")
    else:
        print_color("no changes made", "yellow")

def run_queue_man():
    queue_struct = []
    options = ["add", "peek", "view", "pop", "exit"]
    display_options(options)

    while True:
        choice = input("input one of the valid options: ").strip().lower()
        if choice == "add":
            add(queue_struct)
        elif choice in ("peek", "peek", "view", "pop") and not queue_struct:
            print_color("queue is empty", "red")
        elif choice == "add":
            add(queue_struct)
        elif choice == "peek":
            peek(queue_struct)
        elif choice == "view":
            view(queue_struct)
        elif choice == "pop":
            pop(queue_struct)
        elif choice == "exit":
            break

if __name__ == '__main__':
    run_queue_man()
