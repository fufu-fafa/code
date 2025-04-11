#!/bin/bash

# Define variables
FQBN="esp32:esp32:esp32"
PORT="/dev/cu.usbserial-0001"

# Check if a sketch directory was provided
if [ -z "$1" ]; then
    echo "Usage: up_esp32 <sketch_directory>"
    exit 1
fi

PROJECT_DIR="$1"

# Compile the sketch
arduino-cli compile --fqbn $FQBN "$PROJECT_DIR"

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Uploading..."
    arduino-cli upload -p $PORT --fqbn $FQBN "$PROJECT_DIR"
else
    echo "Compilation failed. Fix errors before uploading."
    exit 1
fi

