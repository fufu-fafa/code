#!/bin/bash

for file in *.ino; do
    name="${file%.ino}"
    mkdir -p "$name"
    mv "$file" "$name"/$file
done
