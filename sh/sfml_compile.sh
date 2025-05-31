#!/bin/bash

if [ -z "$1" ]; then
    echo "usage: sfml_compile {any.cpp}"
    exit 1
fi

FILE="$1"

if [ ! -e "$FILE" ]; then
    echo "no file named '$FILE'"
    exit 1
fi

g++ -std=c++17 $FILE -o sfml.out -I/opt/homebrew/Cellar/sfml/3.0.1/include/ -L/opt/homebrew/Cellar/sfml/3.0.1/lib/ -lsfml-graphics -lsfml-window -lsfml-system
