#!/bin/sh
cd bin && rm -rf mandelbrot mandelbrot_parallel mandelbrot_gui Makefile obj moc_* && \
cd ../src && make clean && \
cd ../output && rm -f *.png
