#!/bin/bash

# Check if correct number of arguments is provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <source_file_or_dir> <number_of_copies>"
    exit 1
fi

source=$1
num_copies=$2

# Check if source exists
if [ ! -e "$source" ]; then
    echo "Error: Source '$source' does not exist."
    exit 1
fi

# Check if number of copies is a valid number
if ! [[ "$num_copies" =~ ^[0-9]+$ ]]; then
    echo "Error: Number of copies must be a positive integer."
    exit 1
fi

# Handle file or directory
if [ -f "$source" ]; then
    # Extract filename without extension and extension separately
    filename="${source%.*}"
    extension="${source##*.}"
    
    # Handle case where there is no extension
    if [ "$filename" == "$source" ]; then
        extension=""
    else
        extension=".$extension"
    fi

    # Create copies
    for i in $(seq 1 "$num_copies"); do
        cp "$source" "${filename}${i}${extension}"
    done
    
    echo "Copied file '$source' $num_copies times."
elif [ -d "$source" ]; then
    # Create copies of directory
    for i in $(seq 1 "$num_copies"); do
        cp -r "$source" "${source}_${i}"
    done
    
    echo "Copied directory '$source' $num_copies times."
else
    echo "Error: '$source' is neither a file nor a directory."
    exit 1
fi

