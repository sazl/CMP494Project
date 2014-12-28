NVCC = nvcc

all: mandelbrot mandelbrot_parallel

mandelbrot: mandelbrot.c lodepng.o
	$(CXX) $(CFLAGS) -o $@ $^ -lm

mandelbrot_parallel: mandelbrot_parallel.cu lodepng_cuda.o
	$(NVCC) $(CFLAGS) -arch=sm_20 -o $@ $^

lodepng.o: lodepng.cpp lodepng.h
	$(CXX) $(CFLAGS) -o $@ -c $<

lodepng_cuda.o: lodepng.cpp lodepng.h
	$(NVCC) $(CFLAGS) -arch=sm_20 -o $@ -c $<

clean:
	rm -f *.o mandelbrot mandelbrot_parallel output.png
