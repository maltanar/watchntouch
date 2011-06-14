/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

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
