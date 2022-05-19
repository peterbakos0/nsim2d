#!/bin/bash

g++ -o main Main.cpp $(pkg-config --libs sdl2) $(pkg-config --cflags sdl2)
