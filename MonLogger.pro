#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T16:01:15
#
#-------------------------------------------------

QT       += core gui widgets
TARGET = MonLogger
TEMPLATE = app
# CONFIG += console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = images/log.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    logger.cpp \
    sshlogger.cpp \
    myplaintextedit.cpp \
    linenumberarea.cpp \
    find.cpp \
    highlighter.cpp \
    sitemanager.cpp \
    simplecrypt.cpp

HEADERS  += mainwindow.h \
    logger.h \
    sshlogger.h \
    myplaintextedit.h \
    linenumberarea.h \
    find.h \
    highlighter.h \
    sitemanager.h \
    simplecrypt.h

FORMS    += mainwindow.ui \
    logfile.ui \
    findwidget.ui \
    sitemanager.ui

LIBS += -L$$PWD/lib/ $$PWD/lib/libssh.dll $$PWD/lib/ssleay32.dll $$PWD/lib/libeay32.dll

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

RESOURCES += \
    qressource.qrc
