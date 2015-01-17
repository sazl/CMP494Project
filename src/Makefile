CUDA_DIR = /usr/local/cuda
NVCC = $(CUDA_DIR)/bin/nvcc
NVCC_FLAGS = -Xptxas -v
CUDA_ARCH = sm_20

all: mandelbrot mandelbrot_parallel

mandelbrot: mandelbrot.c lodepng.o
	$(CXX) $(CFLAGS) -o $@ $^ -lm -DSTANDALONE
mandelbrot_parallel: mandelbrot_parallel.cu lodepng_cuda.o
	$(NVCC) $(CFLAGS) $(NVCC_FLAGS) -arch=$(CUDA_ARCH) -o $@ $^ -DSTANDALONE
lodepng.o: lodepng.cpp lodepng.h
	$(CXX) $(CFLAGS) -o $@ -c $<
lodepng_cuda.o: lodepng.cpp lodepng.h
	$(NVCC) $(CFLAGS) $(NVCC_FLAGS) -arch=$(CUDA_ARCH) -o $@ -c $<
clean:
	rm -f *.o mandelbrot mandelbrot_parallel
