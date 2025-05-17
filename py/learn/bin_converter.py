def convert_input():
    while True:
        try:
            temp = int(input("input a natural number: "))
        except ValueError:
            print("invalid input")
        return str(bin(temp))[2:]

bin_str = convert_input()
print("|" + bin_str.replace("1", "■").replace("0", " ") + "|")
