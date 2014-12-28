#!/bin/sh
./build.sh && \
    ./bin/mandelbrot 1024 1024 1024 output/output_seq.png && \
    ./bin/mandelbrot_parallel 1024 1024 1024 output/output_parallel.png

echo "\n\nDone!"
echo "Output images can be found in output/"
