#!/bin/sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64
cd src && \
    make && \
    cp mandelbrot ../bin/ && \
    cp mandelbrot_parallel ../bin/ && \
    cd ../bin && \
    qmake ../src && \
    make
