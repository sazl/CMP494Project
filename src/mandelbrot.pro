TARGET           = mandelbrot_gui
CONFIG          += qt thread
QT              += gui widgets
DESTDIR          = ../bin
OBJECTS_DIR      = $$DESTDIR/obj
CUDA_OBJECTS_DIR = $$DESTDIR/cuda

HEADERS = mandelbrotwidget.h \
          renderthread.h \
          mandelbrot.h \
          mandelbrot_parallel.h

SOURCES = main.cpp \
          mandelbrotwidget.cpp \
          renderthread.cpp \
          mandelbrot.c \

QMAKE_CFLAGS += -std=c99

CUDA_SOURCES += mandelbrot_parallel.cu \
                lodepng.cpp

CUDA_DIR      = /usr/local/cuda
INCLUDEPATH  += $$CUDA_DIR/include
QMAKE_LIBDIR += $$CUDA_DIR/lib64
LIBS         += -lcuda -lcudart -lm

CUDA_ARCH     = sm_20
NVCC_FLAGS    = --compiler-options -fno-strict-aliasing -use_fast_math

CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')
 
cuda.commands = $$CUDA_DIR/bin/nvcc -m64 -O3 -arch=$$CUDA_ARCH -c $$NVCC_FLAGS \
                $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
 
cuda.dependency_type = TYPE_C
# cuda.depend_command = $$CUDA_DIR/bin/nvcc -O3 -M $$CUDA_INC $$NVCC_FLAGS \
#                      ${QMAKE_FILE_NAME}
cuda.input = CUDA_SOURCES
cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.o
QMAKE_EXTRA_COMPILERS += cuda
