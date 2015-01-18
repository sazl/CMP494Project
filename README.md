CMP 494 Project: Mandelbrot Generator in CUDA
---------------------------------------------

| Name                | ID    |
|---------------------|-------|
| Youssef Elkhorazaty | 45550 |
| Sami Zeinelabdin    | 45619 |

=============================================
Building
=============================================

- Assumes CUDA is installed in /usr/local/cuda

- Assumes a 64bit system and CUDA library

- Please run: sudo ldconfig /usr/local/cuda/lib64
  Before building, otherwise libcudart will not be found!

---------------------------------------------

- To build:

./build.sh

Binaries will be found in the bin/ directory.
The binaries are:

bin/mandelbrot         : Sequential CPU version
bin/mandelbrot_parallel: Parallel GPU CUDA version

---------------------------------------------

- To clean:

./clean.sh

---------------------------------------------

- To test the program run:

./test.sh

This will output 2 files, one from the GPU and one from the CPU in the folder output/


=============================================
GUI
=============================================

The GUI is written in Qt, it can render fractals
using the GPU or the CPU.

The following shortcuts are available:

| Key        | Description               |
|------------|---------------------------|
| R          | Reset fractal position    |
| \+         | Zoom in                   |
| \-         | Zoom out                  |
| i          | Increase iteration count  |
| o          | Decrease iteration count  |
| s          | Take a screenshot, saves as 'out.png' in the present directory |
| Arrow Keys | Move around               |
| Space Bar  | Toggle CPU/GPU            |


=============================================
Directories
=============================================

| Directory | Description               |
|---------|-----------------------------|
| bench/  | Benchmarks files and script |
| bin/    | Binary files                |
| output/ | Output image files          |
| src/    | Project source code         |