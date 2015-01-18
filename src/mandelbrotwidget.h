 #ifndef MANDELBROTWIDGET_H
 #define MANDELBROTWIDGET_H

 #include <QPixmap>
 #include <QWidget>
 #include "renderthread.h"

 class MandelbrotWidget : public QWidget
 {
     Q_OBJECT

 public:
     MandelbrotWidget(QWidget *parent = 0);

 protected:
     void paintEvent(QPaintEvent *event);
     void resizeEvent(QResizeEvent *event);
     void keyPressEvent(QKeyEvent *event);
     void wheelEvent(QWheelEvent *event);
     void closeEvent(QCloseEvent *event);

 private slots:
     void updatePixmap(const QImage &image, double scaleFactor);
     void zoom(double zoomFactor);

 private:
     void reset();
     void scroll(double deltaX, double deltaY);

     RenderThread thread;
     QPixmap pixmap;
     QPoint pixmapOffset;
     QPoint lastDragPos;
     double centerX;
     double centerY;
     double pixmapScale;
     double curScale;
     bool gpuRender;
     int maxIterations;
 };

 #endif
