#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T17:10:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = poisson
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    backwindow.cpp \
    poisson.cpp \
    chooseimg.cpp \
    mymenu.cpp \
    paracontrol.cpp

HEADERS  += mainwindow.h \
    backwindow.h \
    type.h \
    poisson.h \
    chooseimg.h \
    mymenu.h \
    paracontrol.h

FORMS    += \
    mainwindow.ui


#INCLUDEPATH+=H:/opencv/opencv/build/include/opencv \
#        H:/opencv/opencv/build/include/opencv2 \
#                   H:/opencv/opencv/build/include

LIBS+=D:\opencv\build\x64\vc12\lib\opencv_ml2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_calib3d2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_contrib2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_core2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_features2d2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_flann2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_gpu2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_highgui2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_imgproc2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_legacy2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_objdetect2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_ts2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_video2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_nonfree2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_ocl2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_photo2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_stitching2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_superres2413.lib\
D:\opencv\build\x64\vc12\lib\opencv_videostab2413.lib\

INCLUDEPATH += d:\opencv\build\include\
INCLUDEPATH += d:\opencv\build\include\opencv\
INCLUDEPATH += d:\opencv\build\include\opencv2\
