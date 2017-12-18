#-------------------------------------------------
#
# Project created by QtCreator 2017-12-17T21:52:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FAMLab
TEMPLATE = app


HEADERS += \
        MAELab/imageModel/*.h \
        MAELab/io/*.h \
        MAELab/io/LibJpeg/*.h \
        MAELab/segmentation/*.h \
        MAELab/utils/*.h \
        matching/*.h \
        segmentation/*.h \
        ui/*.h

SOURCES += \
        MAELab/imageModel/*.cpp \
        MAELab/io/*.cpp \
        MAELab/io/LibJpeg/*.c \
        MAELab/segmentation/*.cpp \
        MAELab/utils/*.cpp \
        matching/*.cpp \
        segmentation/*.cpp \
        ui/*.cpp \
        main.cpp
