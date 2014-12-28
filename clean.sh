#!/bin/sh
cd bin && rm -f mandelbrot mandelbrot_parallel && cd .. && \
cd src && make clean && cd .. && \
cd output && rm -f *.png
