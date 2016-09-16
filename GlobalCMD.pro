#-------------------------------------------------
#
# Project created by QtCreator 2016-09-13T14:11:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GlobalCMD
TEMPLATE = app


SOURCES += main.cpp\
        globalcmd.cpp \
    filelistmodel.cpp \
    drivescombobox.cpp \
    filelistpanel.cpp \
    gcmdtableview.cpp

HEADERS  += globalcmd.h \
    filelistmodel.h \
    drivescombobox.h \
    filelistpanel.h \
    gcmdtableview.h

FORMS    += globalcmd.ui \
    filelistpanel.ui
