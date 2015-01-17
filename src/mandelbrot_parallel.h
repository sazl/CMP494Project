#ifndef MANDELBROT_PARALLEL_H
#define MANDELBROT_PARALLEL_H

void run_mandelbrot_parallel(unsigned char** out_image,
                             size_t* image_size,
                             unsigned int width,
                             unsigned int height,
                             unsigned int max_iterations,
                             unsigned int block_dim_x,
                             unsigned int block_dim_y,
                             float x_scale, float y_scale,
                             float x_adjust, float y_adjust);

#endif
