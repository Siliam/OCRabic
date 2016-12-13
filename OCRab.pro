#-------------------------------------------------
#
# Project created by QtCreator 2016-12-10T16:23:47
#   By Ismail
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OCRab
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            cvworker.cpp \
            nnlearner.cpp \
            image.cpp \
            knnlearner.cpp

HEADERS  += mainwindow.h \
            cvworker.h \
            nnlearner.h \
            image.h \
            knnlearner.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:\\OpenCV\\install\\include

LIBS += -LC:\\OpenCV\\install\\x64\\vc14\\lib \
        -lopencv_core310d \
        -lopencv_imgproc310d \
        -lopencv_highgui310d \
        -lopencv_imgcodecs310d
