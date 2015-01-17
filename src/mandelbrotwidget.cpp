#include <QtGui>

#include <cmath>

#include "mandelbrotwidget.h"

const double DefaultCenterX = 2.0f;
const double DefaultCenterY = 1.5f;
const double DefaultScale = 3.0f;

const int DefaultMaxIterations = 1000;

const double ZoomInFactor = 0.9f;
const double ZoomOutFactor = 1 / ZoomInFactor;
const double ScrollStep = 0.1;

MandelbrotWidget::MandelbrotWidget(QWidget *parent)
        : QWidget(parent)
{
    centerX = DefaultCenterX;
    centerY = DefaultCenterY;
    pixmapScale = DefaultScale;
    curScale = DefaultScale;
    maxIterations = DefaultMaxIterations;
    gpuRender = true;

    qRegisterMetaType<QImage>("QImage");
    connect(&thread, SIGNAL(renderedImage(QImage,double)),
            this, SLOT(updatePixmap(QImage,double)));

    setWindowTitle(tr("Mandelbrot"));
    resize(1024, 1024);
}

void MandelbrotWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (pixmap.isNull()) {
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter,
                         tr("Rendering initial image, please wait..."));
        return;
    }

    if (curScale == pixmapScale) {
        painter.drawPixmap(pixmapOffset, pixmap);
    } else {
        double scaleFactor = pixmapScale / curScale;
        int newWidth = int(pixmap.width() * scaleFactor);
        int newHeight = int(pixmap.height() * scaleFactor);
        int newX = pixmapOffset.x() + (pixmap.width() - newWidth) / 2;
        int newY = pixmapOffset.y() + (pixmap.height() - newHeight) / 2;
        painter.save();
        painter.translate(newX, newY);
        painter.scale(scaleFactor, scaleFactor);
        QRectF exposed = painter.matrix().inverted().mapRect(rect()).adjusted(-1, -1, 1, 1);
        painter.drawPixmap(exposed, pixmap, exposed);
        painter.restore();
    }
}

void MandelbrotWidget::resizeEvent(QResizeEvent * /* event */)
{
    thread.render(centerX, centerY, curScale, size(), gpuRender, maxIterations);
}

void MandelbrotWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_S:
            pixmap.save("out.png", "PNG");
            break;
        case Qt::Key_R:
            reset();
            break;
        case Qt::Key_I:
            maxIterations += 100;
            update();
            thread.render(centerX, centerY, curScale, size(), gpuRender, maxIterations);
            break;
        case Qt::Key_O:
            maxIterations -= 100;
            update();
            thread.render(centerX, centerY, curScale, size(), gpuRender, maxIterations);
            break;
        case Qt::Key_Space:
            gpuRender = !gpuRender;
            update();
            thread.render(centerX, centerY, curScale, size(), gpuRender, maxIterations);
            break;
        case Qt::Key_Plus:
            zoom(ZoomInFactor);
            break;
        case Qt::Key_Minus:
            zoom(ZoomOutFactor);
            break;
        case Qt::Key_Left:
            scroll(+ScrollStep, 0);
            break;
        case Qt::Key_Right:
            scroll(-ScrollStep, 0);
            break;
        case Qt::Key_Down:
            scroll(0, -ScrollStep);
            break;
        case Qt::Key_Up:
            scroll(0, +ScrollStep);
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}

void MandelbrotWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    double numSteps = numDegrees / 15.0f;
    zoom(pow(ZoomInFactor, numSteps));
}

void MandelbrotWidget::updatePixmap(const QImage &image, double scaleFactor)
{
    if (!lastDragPos.isNull())
        return;

    pixmap = QPixmap::fromImage(image);
    pixmapOffset = QPoint();
    lastDragPos = QPoint();
    pixmapScale = scaleFactor;
    update();
}

void MandelbrotWidget::reset()
{
    centerX = DefaultCenterX;
    centerY = DefaultCenterY;
    pixmapScale = DefaultScale;
    curScale = DefaultScale;
    update();
    thread.render(centerX, centerY, curScale, size(), gpuRender, maxIterations);
}

void MandelbrotWidget::zoom(double zoomFactor)
{
    curScale *= zoomFactor;
    update();
    thread.render(centerX, centerY, curScale, size(), gpuRender, maxIterations);
}

void MandelbrotWidget::scroll(double deltaX, double deltaY)
{
    centerX += deltaX * curScale;
    centerY += deltaY * curScale;
    update();
    thread.render(centerX, centerY, curScale, size(), gpuRender, maxIterations);
}
