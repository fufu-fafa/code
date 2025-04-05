#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <file.gpg>"
    exit 1
fi

encrypted_file="$1"
decrypted_file="$(mktemp)"

# Decrypt directly to file
if ! gpg --output "$decrypted_file" --decrypt "$encrypted_file" 2>/dev/null; then
    echo "Error: Failed to decrypt '$encrypted_file'."
    rm -f "$decrypted_file"
    exit 1
fi

# Check if text
if file --mime-type "$decrypted_file" | grep -q 'text/'; then
    cat "$decrypted_file"
else
    echo "Error: Decrypted file is not a readable text file."
fi

rm -f "$decrypted_file"

