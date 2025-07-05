#!/bin/sh
g++ src/* -o main $("pkg-config --cflags --libs opencv4")