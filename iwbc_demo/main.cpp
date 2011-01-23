#include <QtGui/QApplication>
#include "mainwindow.h"

#include "recentlyused.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
