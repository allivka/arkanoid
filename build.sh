#!/bin/sh
g++ src/* -o build/main $("pkg-config --cflags --libs opencv4")