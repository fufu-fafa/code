string_input = input("input a word to check for length: ").strip()
str_len = len(list(string_input))
out_print = "base 10: " + str(str_len) + ", base 16: " + str(hex(str_len)[2:]) + ", base 2: " + str(bin(str_len)[2:])
print(out_print)