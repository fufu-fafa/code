num = 0
while True:
    temp = input("press enter to continue...").strip().lower()
    if temp == "exit":
        break
    num += 1
    print(bin(num)[2:])
