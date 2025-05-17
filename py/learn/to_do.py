def addition(to_do):
    task = input("task to be added: ")
    to_do += [task]

def enum_list(to_do):
    if not to_do:
        print("to do list is empty")
        return
    for i, item in enumerate(to_do, start=1):
        print(f"{i}. {item}")

def remove(to_do):
    which_task = input("input the name or index number of the task: ")
    try:
        remove_index = int(which_task) - 1
        if remove_index < 0 or remove_index + 1 > len(to_do):
            print("invalid index")
            return
        to_do.pop(remove_index)
    except ValueError:
        if which_task in to_do:
            to_do.remove(which_task)
        else:
            print("task not found")

def get_input():
    try:
        temp = int(input("input the number of the option: "))
        return temp
    except ValueError:
        print("invalid input")
        return None

def main():
    to_do = []
    while True:
        print("options:")
        print("1. add task into list")
        print("2. remove a task from the list")
        print("3. show list")
        print("4. exit")
        num_chosen = get_input()
        if num_chosen == 1:
            addition(to_do)
        elif num_chosen == 2:
            enum_list(to_do)
            remove(to_do)
        elif num_chosen == 3:
            print("to do list:")
            enum_list(to_do)
        elif num_chosen == 4:
            break
        else:
            print("invalid option")
        print("")

if __name__ == "__main__":
    main()
