import base64
from cust_col import print_color

def to_bin(text):
    return ' '.join(format(ord(x), '08b') for x in text)

def from_bin(encoded):
    bin_val = encoded.split()
    ascii_char = [chr(int(n, 2)) for n in bin_val]
    return ''.join(ascii_char)

def to_base64(text):
    temp = text.encode('utf-8')
    return base64.b64encode(temp).decode('utf-8')

def from_base64(encoded):
    padding = len(encoded) % 4
    if padding:
        encoded += '=' * (4 - padding)
    encoded_bytes = base64.b64decode(encoded)
    return encoded_bytes.decode('utf-8')

try:
    options = input("'encode' or 'decode': ").strip().lower()
    if options not in ('encode', 'decode'):
        print_color("invalid input", "red")
    elif options == "encode":
        enc = input("input the text to encode: ")
        temp = to_bin(enc)
        result = to_base64(temp)
        print(result)
    else:
        decode = input("input the text to decode: ")
        temp = from_base64(decode)
        result = from_bin(temp)
        print(result)
except Exception as e:
    print_color(f"err: {e}", "red")
