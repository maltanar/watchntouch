#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "presentationdisplaywidget.h"
#include "basedrawingwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PresentationDisplayWidget *display;
    BaseDrawingWidget *draw;

public slots:
    void contentChange(QString newcontent);
    void contextChange(QString newcontext);
};

#endif // MAINWINDOW_H
