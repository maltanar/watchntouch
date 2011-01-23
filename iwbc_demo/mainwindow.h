#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "presentationdisplaywidget.h"
#include "annotationwidget.h"

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
    AnnotationWidget *draw;

    void openContent();

public slots:


private slots:
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
