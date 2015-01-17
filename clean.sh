#!/bin/sh
cd bin && rm -rf mandelbrot mandelbrot_parallel Makefile obj && \
cd ../src && make clean && \
cd ../output && rm -f *.png
