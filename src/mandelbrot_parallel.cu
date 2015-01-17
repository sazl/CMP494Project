#include <stdio.h>
#include <stdlib.h>

#include <cuda.h>

#include "lodepng.h"
#include "mandelbrot_parallel.h"

typedef unsigned int uint;
typedef unsigned char uchar;

// Linear interpolation
#define LERP(x, y, t) \
    ((1-t)*x + t*y)

const uint BYTES_PER_PIXEL = 3;
const uint LIMIT           = (1 << 16);

__global__ void render(uchar* image,
                       uint width,
                       uint height,
                       uint max_iterations,
                       float x_scale,
                       float y_scale,
                       float x_adjust,
                       float y_adjust)
{
    uint x_dim = blockIdx.x*blockDim.x + threadIdx.x;
    uint y_dim = blockIdx.y*blockDim.y + threadIdx.y;
    uint index = BYTES_PER_PIXEL * (width * y_dim + x_dim);
    float x_origin = ((float) x_dim/width)*x_scale - x_adjust;
    float y_origin = ((float) y_dim/width)*y_scale - y_adjust;

    float x = 0.0;
    float y = 0.0;
    uint iteration = 0;

    // Escape time algorithm
    while(x*x + y*y < LIMIT && iteration < max_iterations) {
        float xtemp = x*x - y*y + x_origin;
        y = 2*x*y + y_origin;
        x = xtemp;
        iteration++;
    }

    if (iteration == max_iterations) {
        image[index]     = 0;
        image[index + 1] = 0;
        image[index + 2] = 0;
    }
    else {
        // Continous coloring
        // Computes the color as a linear interpolation of surrounding points
        // smoothing the color transition.
        float zn  = sqrt(x*x + y*y);
        float nu  = log10f(log10f(zn) / log10f(2)) / log10f(2);
        float itr = ((float) iteration) + 1 - nu;
        float t   = fmodf(itr, 1.0);
        iteration = (uint) itr;
        uint color1 = iteration;
        uint color2 = iteration+1;

        // Assign RGB values by multiplying the iteration count by a even multiples.
        // Should replace this with a predefined colormap.
        image[index]     = ((uint) LERP(color1, color2, t)*2) % max_iterations;
        image[index + 1] = ((uint) LERP(color1, color2, t)*4) % max_iterations;
        image[index + 2] = ((uint) LERP(color1, color2, t)*6) % max_iterations;
    }
}

void run_mandelbrot_parallel(uchar** out_image,
                             size_t* image_size,
                             uint width, uint height,
                             uint max_iterations,
                             uint block_dim_x, uint block_dim_y,
                             float x_scale,
                             float y_scale,
                             float x_adjust,
                             float y_adjust)
{
    size_t buffer_size = sizeof(uchar) * width * height * BYTES_PER_PIXEL;
    uchar* image;
    cudaMalloc((void**) &image, buffer_size);
    uchar* host_image = (uchar*) malloc(buffer_size);

    // Render image
    dim3 blockDim(block_dim_x, block_dim_y);
    dim3 gridDim(width / blockDim.x, height / blockDim.y);
    render<<<gridDim, blockDim>>>(image, width, height, max_iterations,
                                  x_scale, y_scale,
                                  x_adjust, y_adjust);

    // Copy device image to host and output to png file
    cudaMemcpy(host_image, image, buffer_size, cudaMemcpyDeviceToHost);
    cudaFree(image);

    // Output image and size
    *out_image = host_image;
    *image_size = buffer_size;
}

#ifdef STANDALONE

// Scales the x-axis and y-axis, centers the mandelbrot
const float X_SCALE = 3.25;
const float Y_SCALE = 2.5;
const float X_ADJUST = 2;
const float Y_ADJUST = 1.25;

void save_mandelbrot(const char* file_name,
                     uint width, uint height,
                     uint max_iterations,
                     uint block_dim_x, uint block_dim_y)
{
    uchar *host_image;
    size_t image_size;
    run_mandelbrot_parallel(&host_image, &image_size,
                            width, height, max_iterations,
                            block_dim_x, block_dim_y,
                            X_SCALE, Y_SCALE,
                            X_ADJUST, Y_ADJUST);
    lodepng_encode24_file(file_name, host_image, width, height);
    free(host_image);
}

int main(int argc, const char* argv[]) {

    if (argc != 7) {
        fprintf(stderr, "%s [width] [height] [max_iterations] [block_dim_x] [block_dim_y] [file name]\n", argv[0]);
        return 1;
    }
    
    uint width = (uint) atoi(argv[1]);
    uint height = (uint) atoi(argv[2]);
    uint max_iterations = (uint) atoi(argv[3]);
    uint block_dim_x = (uint) atoi(argv[4]);
    uint block_dim_y = (uint) atoi(argv[5]);
    const char* file_name = argv[6];
    save_mandelbrot(file_name,
                    width, height,
                    max_iterations,
                    block_dim_x, block_dim_y);
    return 0;
}

#endif