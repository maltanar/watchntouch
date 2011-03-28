#include <QtGui/QApplication>
#include "appglobals.h"
#include "mainwindow.h"
#include "calibrationwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    CalibrationWindow c;

    w.connect(&c, SIGNAL(initComplete()), SLOT(showFullScreen()));

    c.showFullScreen();

    return a.exec();
}

void displayErrorMessage(QString messageText, QString messageTitle)
{
    QMessageBox err;

    err.setWindowTitle(messageTitle);
    err.setText(messageText);
    err.setIcon(QMessageBox::Critical);
    err.setDefaultButton(QMessageBox::Ok);
    err.exec();
}

void displayInfoMessage(QString messageText, QString messageTitle)
{
    QMessageBox inf;

    inf.setWindowTitle(messageTitle);
    inf.setText(messageText);
    inf.setIcon(QMessageBox::Information);
    inf.setDefaultButton(QMessageBox::Ok);
    inf.exec();
}
