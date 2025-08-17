#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: compileSFML <sfmlApp.cpp>"
    exit 1
fi

PROJECT_DIR="$1"
g++ -std=c++23 -I/opt/homebrew/Cellar/sfml/3.0.1/include -L/opt/homebrew/Cellar/sfml/3.0.1/lib -lsfml-graphics -lsfml-window -lsfml-system $1
