#-------------------------------------------------
#
# Project created by QtCreator 2015-04-21T22:20:24
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WallpaperChanger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    classes/appsettings.cpp

HEADERS  += mainwindow.h \
    classes/appsettings.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
