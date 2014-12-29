#!/bin/sh
./build.sh && \
    time ./bin/mandelbrot 1024 1024 1024 output/output_seq.png && \
    time ./bin/mandelbrot_parallel 1024 1024 1024 output/output_parallel.png

echo "\n\nDone!"
echo "Output images can be found in output/"
