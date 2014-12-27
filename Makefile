CC = nvcc

all: mandelbrot

mandelbrot: main.cu lodepng.o
	$(CC) $(CFLAGS) -arch=sm_20 -O2 -o $@ $^

lodepng.o: lodepng.cpp lodepng.h
	$(CC)  -arch=sm_20 -O2 -c $<

clean:
	rm -f *.o mandelbrot output.png
