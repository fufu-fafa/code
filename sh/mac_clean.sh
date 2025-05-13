#!/bin/bash

# Check if a directory argument is given
if [ -z "$1" ]; then
    echo "Usage: mac_clean {directory}"
    exit 1
fi

TARGET_DIR="$1"

# Check if the directory exists
if [ ! -d "$TARGET_DIR" ]; then
    echo "Error: '$TARGET_DIR' is not a directory."
    exit 1
fi

echo "Cleaning .DS_Store and ._* files from '$TARGET_DIR'..."
find "$TARGET_DIR" -name '.DS_Store' -type f -print -delete
find "$TARGET_DIR" -name '._*' -type f -print -delete
