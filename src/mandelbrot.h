#ifndef MANDELBROT_H
#define MANDELBROT_H

#ifdef __cplusplus
extern "C" {
#endif

void run_mandelbrot(unsigned char** out_image,
                        size_t* image_size,
                        unsigned int width, unsigned int height,
                        unsigned int max_iterations,
                        float x_scale, float y_scale,
                        float x_adjust, float y_adjust);

#ifdef __cplusplus
}
#endif

#endif
