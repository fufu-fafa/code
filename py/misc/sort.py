import sys

def sort(input, output):
    try:
        with open(input, 'r') as infile:
            uniq = set(infile.readlines())
        done = sorted(uniq)
        with open(output, 'w') as outfile:
            outfile.writelines(done)
        print(f"Done. Output in {output}")
    except FileNotFoundError:
        print(f"no {input} file found")
    except Exception as e:
        print(f"error: {e}")

if __name__=="__main__":
    if len(sys.argv) != 2:
        print("usage: python sort.py <file>")
        sys.exit(1)
    input = sys.argv[1]
    output = "sorted_" + input
    sort(input, output)
