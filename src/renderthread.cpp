#include <QtGui>

#include <cmath>
#include <iostream>

#include "renderthread.h"
#include "mandelbrot.h"
#include "mandelbrot_parallel.h"

RenderThread::RenderThread(QObject *parent)
        : QThread(parent)
{
    restart = false;
    abort = false;
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();
    wait();
}

void RenderThread::render(double centerX, double centerY, double scaleFactor,
                          QSize resultSize, bool gpuRender, int maxIterations)
{
    QMutexLocker locker(&mutex);

    this->centerX = centerX;
    this->centerY = centerY;
    this->scaleFactor = scaleFactor;
    this->resultSize = resultSize;
    this->maxIterations = maxIterations;
    this->gpuRender = gpuRender;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

void RenderThread::run()
{
    forever {
        mutex.lock();
        QSize resultSize = this->resultSize;
        double scaleFactor = this->scaleFactor;
        float x_scale = scaleFactor;
        float y_scale = scaleFactor;
        double centerX = this->centerX;
        double centerY = this->centerY;
        float x_adjust = centerX;
        float y_adjust = centerY;
        int max_iterations = this->maxIterations;
        mutex.unlock();
        int width = resultSize.width();
        int height = resultSize.height();
        uchar* image_bits;
        size_t image_size;

        if (this->gpuRender) {
            run_mandelbrot_parallel(&image_bits, &image_size,
                                    width, height,
                                    max_iterations,
                                    16, 16,
                                    scaleFactor, scaleFactor,
                                    centerX, centerY);
        }
        else {
            run_mandelbrot(&image_bits,
                           &image_size,
                           width, height,
                           max_iterations,
                           x_scale, y_scale,
                           x_adjust, y_adjust);
        }
        QImage image(image_bits, width, height, QImage::Format_RGB32);
        emit renderedImage(image, scaleFactor);                           

        mutex.lock();
        if (!restart) {
            condition.wait(&mutex);
        }
        restart = false;
        delete[] image_bits;
        mutex.unlock();
    }
}
