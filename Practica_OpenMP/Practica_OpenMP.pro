#-------------------------------------------------
#
# Project created by QtCreator 2015-05-06T17:46:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Practica_OpenMP
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            menu.cpp

HEADERS  += mainwindow.h \
            menu.h


LIBS += `pkg-config opencv --libs`

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

DISTFILES += \
    image1.png \
    image2.png \
    image3.png \

FORMS    += mainwindow.ui
