#include <QtGui/QApplication>
#include "mainwindow.h"
#include "calibrationwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    CalibrationWindow c;

    w.connect(&c, SIGNAL(initComplete()), SLOT(showFullScreen()));

    w.showFullScreen();

    return a.exec();
}
