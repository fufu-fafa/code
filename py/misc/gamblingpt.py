import random
from time import sleep


def get_parity(num):
    """Return 0 if even, 1 if odd."""
    return num % 2


def main():
    # Prompt user for input
    pick = input("Pick 'e' for even or 'o' for odd: ").lower()
    if pick not in ("e", "o"):
        print("Invalid input. Please choose 'e' or 'o'.")
        return

    # Set expected parity based on user input
    user_parity = 0 if pick == "e" else 1

    # Generate random number
    random_num = random.randint(1, 1000000)  # Reduced range for practicality
    num_parity = get_parity(random_num)

    # Countdown
    print("Checking...")
    for i in range(3, 0, -1):
        print(i)
        sleep(1)

    # Check result
    if num_parity == user_parity:
        print("TRUE! The number was:", random_num)
    else:
        print("FALSE! The number was:", random_num)


if __name__ == "__main__":
    main()
