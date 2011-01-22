#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T13:21:25
#
#-------------------------------------------------

QT       += core gui svg

TARGET = iwbc_demo
TEMPLATE = app

INCLUDEPATH  += /usr/include/poppler/qt4
LIBS         += -L/usr/lib -lpoppler-qt4

SOURCES += main.cpp\
        mainwindow.cpp \
    contentdisplay.cpp \
    presentationdisplaywidget.cpp \
    contentmatcher.cpp \
    drawingdata.cpp \
    drawingaction.cpp \
    basedrawingwidget.cpp \
    transparentstackedlayout.cpp \
    annotationwidget.cpp

HEADERS  += mainwindow.h \
    contentdisplay.h \
    presentationdisplaywidget.h \
    contentmatcher.h \
    appglobals.h \
    drawingdata.h \
    drawingaction.h \
    basedrawingwidget.h \
    transparentstackedlayout.h \
    annotationwidget.h

FORMS    += mainwindow.ui
