#-------------------------------------------------
#
# Project created by QtCreator 2016-09-13T14:11:01
#
#-------------------------------------------------

QT       += core gui
QT       += concurrent


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GlobalCMD
TEMPLATE = app


SOURCES += main.cpp\
        globalcmd.cpp \
    filelistmodel.cpp \
    drivescombobox.cpp \
    filelistpanel.cpp \
    gcmdtableview.cpp \
    fileoperator.cpp \
    settingsmanager.cpp \
    settingswindow.cpp \
    Pages/fontspage.cpp \
    Pages/columnspage.cpp

HEADERS  += globalcmd.h \
    filelistmodel.h \
    drivescombobox.h \
    filelistpanel.h \
    gcmdtableview.h \
    fileoperator.h \
    settingsmanager.h \
    settingswindow.h \
    Pages/fontspage.h \
    Pages/columnspage.h

FORMS    += globalcmd.ui \
    filelistpanel.ui \
    settingswindow.ui \
    Pages/fontspage.ui \
    Pages/columnspage.ui

RESOURCES += \
    resources.qrc
