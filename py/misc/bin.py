import argparse

def file_check(output_file):
    try:
        with open(output_file, 'r') as check:
            check.read()
            return True
    except FileNotFoundError:
        return False

def text_to_binary(text):
    return ' '.join(format(ord(char), '08b') for char in text)

def binary_to_text(binary):
    try:
        return ''.join(chr(int(b, 2)) for b in binary.split())
    except ValueError:
        return "Invalid binary input. Ensure each binary chunk is 8 bits."

def encode_file(input_file, output_file):
    if file_check(output_file):
        print(f"error: {output_file} already exist")
        return
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            text = f.read()
        binary_data = text_to_binary(text)
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(binary_data)
    except FileNotFoundError:
        print(f"error: {input_file} not found")
    except Exception as e:
        print(f"error: {e}")

def decode_file(input_file, output_file):
    if file_check(output_file):
        print(f"error: {output_file} already exist")
        return
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            binary_data = f.read()
        text = binary_to_text(binary_data)
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(text)
    except FileNotFoundError:
        print(f"error: {input_file} not found")
    except Exception as e:
        print(f"error: {e}")

def main():
    parser = argparse.ArgumentParser(description="Encode or decode text using binary.")
    parser.add_argument("-e", "--encode", action="store_true", help="Encode file or text to binary.")
    parser.add_argument("-d", "--decode", action="store_true", help="Decode binary to text.")
    parser.add_argument("-f", "--file", help="Input file to encode or decode.")
    parser.add_argument("-o", "--output", help="Output file to save the result.")
    args = parser.parse_args()
    
    if args.file and args.output:
        if args.encode:
           encode_file(args.file, args.output)
        elif args.decode:
            decode_file(args.file, args.output)
        else:
            print("Please specify whether to encode (-e) or decode (-d).")
    else:
        print("Please provide -e to encode or -d to decode, along with a file (-f) and output (-o).")

if __name__ == "__main__":
    main()
