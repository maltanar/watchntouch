#include <QtGui/QApplication>
#include "mainwindow.h"
#include <messagetransceiver.h>
#include <protocolhandler.h>
#include <collaborationclient.h>

#include "collaborationsession.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
