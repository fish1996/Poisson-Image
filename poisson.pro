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
    poisson.cpp

HEADERS  += mainwindow.h \
    backwindow.h \
    poisson.h \
    type.h

FORMS    += mainwindow.ui


#INCLUDEPATH+=H:/opencv/opencv/build/include/opencv \
#        H:/opencv/opencv/build/include/opencv2 \
#                   H:/opencv/opencv/build/include

#LIBS+= H:\opencv\opencv\build\x86\vc10\lib\opencv_ml2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_calib3d2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_contrib2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_core2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_features2d2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_flann2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_gpu2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_highgui2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_imgproc2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_legacy2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_objdetect2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_ts2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_video2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_nonfree2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_ocl2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_photo2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_stitching2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_superres2410.lib\
#H:\opencv\opencv\build\x86\vc10\lib\opencv_videostab2410.lib\

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib \
     -lopencv_core \
     -lopencv_highgui \
     -lopencv_imgproc \
