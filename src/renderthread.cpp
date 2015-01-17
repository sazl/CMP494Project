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

    for (int i = 0; i < ColormapSize; ++i)
        colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
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
                          QSize resultSize)
{
    QMutexLocker locker(&mutex);

    this->centerX = centerX;
    this->centerY = centerY;
    this->scaleFactor = scaleFactor;
    this->resultSize = resultSize;

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
        int max_iterations = 1000;
        mutex.unlock();

        int width = resultSize.width();
        int height = resultSize.height();
        QImage image(resultSize, QImage::Format_RGB888);
        uchar* image_bits = image.bits();
        size_t image_size;

        run_mandelbrot(&image_bits,
                       &image_size,
                       width, height,
                       max_iterations,
                       x_scale, y_scale,
                       x_adjust, y_adjust);

        std::cout << x_scale << '\t'
                  << y_scale << '\t'
                  << x_adjust << '\t'
                  << y_adjust << '\n';
        // run_mandelbrot_parallel(&image_bits, &image_size,
        //                         width, height,
        //                         max_iterations,
        //                         16, 16,
        //                         scaleFactor, scaleFactor,
        //                         centerX, centerY);

        emit renderedImage(image, scaleFactor);
                                   

        mutex.lock();
        if (!restart) {
            condition.wait(&mutex);
        }
        restart = false;
        mutex.unlock();
    }
}

uint RenderThread::rgbFromWaveLength(double wave)
{
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    if (wave >= 380.0 && wave <= 440.0) {
        r = -1.0 * (wave - 440.0) / (440.0 - 380.0);
        b = 1.0;
    } else if (wave >= 440.0 && wave <= 490.0) {
        g = (wave - 440.0) / (490.0 - 440.0);
        b = 1.0;
    } else if (wave >= 490.0 && wave <= 510.0) {
        g = 1.0;
        b = -1.0 * (wave - 510.0) / (510.0 - 490.0);
    } else if (wave >= 510.0 && wave <= 580.0) {
        r = (wave - 510.0) / (580.0 - 510.0);
        g = 1.0;
    } else if (wave >= 580.0 && wave <= 645.0) {
        r = 1.0;
        g = -1.0 * (wave - 645.0) / (645.0 - 580.0);
    } else if (wave >= 645.0 && wave <= 780.0) {
        r = 1.0;
    }

    double s = 1.0;
    if (wave > 700.0)
        s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
    else if (wave <  420.0)
        s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);

    r = pow(r * s, 0.8);
    g = pow(g * s, 0.8);
    b = pow(b * s, 0.8);
    return qRgb(int(r * 255), int(g * 255), int(b * 255));
}
