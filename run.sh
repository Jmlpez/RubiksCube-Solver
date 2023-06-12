#!/bin/bash
g++ src/*.cpp src/RubiksCube/*.cpp  main.cpp -o app glad.o -lglfw -lassimp -ldl && ./app
exit 1
