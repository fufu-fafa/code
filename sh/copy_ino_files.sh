#!/bin/bash

for file in *.ino; do
    name="${file%.ino}"
    mkdir -p "$name"
    cp "$file" "$name"/$file
done
