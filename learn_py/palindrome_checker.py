from cust_col import print_color

in_str = input("input the word to check if palidrome: ").strip()
rev_str = in_str[::-1]
if in_str == rev_str:
    print_color("palindrome", "green")
else:
    print_color("not palindrome", "red")
