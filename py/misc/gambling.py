import random
from time import sleep

pick = input("pick e/o: ")
value = None
this = random.randint(1, 898752385838732587987953)
if this % 2 == 0:
    valeu = 0
else:
    valeu = 1
if pick == "e":
    value = 0
    print("checking")
elif pick == "o":
    value = 1
    print("checking")
else:
    print("WRONG")
    exit()

print("3")
sleep(1)
print("2")
sleep(1)
print("1")
sleep(1)
if valeu == value:
    print("TRUE")
    print(this)
else:
    print("FALSE")
    print(this)
