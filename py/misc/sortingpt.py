# Python script to remove duplicate lines and sort them
import sys

def sort_and_deduplicate(input_file, output_file):
    try:
        with open(input_file, 'r') as infile:
            # Read lines and remove duplicates using a set
            unique_lines = set(infile.readlines())

        # Sort the unique lines
        sorted_lines = sorted(unique_lines)

        # Write the sorted lines to the output file
        with open(output_file, 'w') as outfile:
            outfile.writelines(sorted_lines)

        print(f"Processing complete. Output written to {output_file}")
    except FileNotFoundError:
        print(f"Error: The file {input_file} does not exist.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

# Example usage
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python sort_dedup.py <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = "sorted_" + input_file  # Output file will have a prefix "sorted_"
    sort_and_deduplicate(input_file, output_file)
