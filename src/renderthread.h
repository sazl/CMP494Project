 #ifndef RENDERTHREAD_H
 #define RENDERTHREAD_H

 #include <QMutex>
 #include <QSize>
 #include <QThread>
 #include <QWaitCondition>

 class QImage;

 class RenderThread : public QThread
 {
     Q_OBJECT

 public:
     RenderThread(QObject *parent = 0);
     ~RenderThread();

     void render(double centerX, double centerY, double scaleFactor,
                 QSize resultSize, bool gpuRender, int maxIterations);

 signals:
     void renderedImage(const QImage &image, double scaleFactor);

 protected:
     void run();

 private:
     QMutex mutex;
     QWaitCondition condition;
     double centerX;
     double centerY;
     double scaleFactor;
     QSize resultSize;
     bool gpuRender;
     int maxIterations;
     bool restart;
     bool abort;
 };

 #endif
