#-------------------------------------------------
#
# Project created by QtCreator 2015-05-06T17:46:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Practica_OpenMP
TEMPLATE = app
INCLUDEPATH += "/usr/local/include/opencv2"

SOURCES +=  main.cpp\
            mainwindow.cpp \
            menu.cpp \
            histcomp.cpp \
            histextract.cpp

HEADERS  += mainwindow.h \
            menu.h

LIBS += `pkg-config opencv --libs`

#QMAKE_CXXFLAGS+= -openmp
#QMAKE_LFLAGS +=  -openmp


QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS   += -fopenmp

FORMS    += mainwindow.ui
