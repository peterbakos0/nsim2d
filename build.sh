#!/bin/bash

g++ $(pkg-config --libs sdl2) $(pkg-config --cflags sdl2) -o main Main.cpp
