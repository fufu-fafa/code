import argparse

def text_to_binary(text):
    return ' '.join(format(ord(char), '08b') for char in text)

def binary_to_text(binary):
    try:
        return ''.join(chr(int(b, 2)) for b in binary.split())
    except ValueError:
        return "Invalid binary input. Ensure each binary chunk is 8 bits."

def encrypt_file(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as f:
        text = f.read()
    binary_data = text_to_binary(text)
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(binary_data)

def decrypt_file(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as f:
        binary_data = f.read()
    text = binary_to_text(binary_data)
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(text)

def main():
    parser = argparse.ArgumentParser(description="Encrypt or decrypt text using binary encoding.")
    parser.add_argument("-e", "--encrypt", action="store_true", help="Encrypt file or text to binary.")
    parser.add_argument("-d", "--decrypt", action="store_true", help="Decrypt binary to text.")
    parser.add_argument("-f", "--file", help="Input file to encrypt or decrypt.")
    parser.add_argument("-o", "--output", help="Output file to save the result.")
    
    args = parser.parse_args()
    
    if args.file and args.output:
        if args.encrypt:
            encrypt_file(args.file, args.output)
        elif args.decrypt:
            decrypt_file(args.file, args.output)
        else:
            print("Please specify whether to encrypt (-e) or decrypt (-d).")
    else:
        print("Please provide -e to encrypt or -d to decrypt, along with a file (-f) and output (-o).")

if __name__ == "__main__":
    main()
