 #include <QApplication>

 #include "mandelbrotwidget.h"

 int main(int argc, char *argv[])
 {
     QApplication app(argc, argv);
     MandelbrotWidget widget;
     app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
     widget.show();
     return app.exec();
 }
