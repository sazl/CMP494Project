

HEADERS       = mandelbrotwidget.h \
                 renderthread.h
SOURCES       = main.cpp \
                 mandelbrotwidget.cpp \
                 renderthread.cpp

 unix:!mac:!symbian:!vxworks:!integrity:LIBS += -lm

CONFIG += qt thread
QT += gui