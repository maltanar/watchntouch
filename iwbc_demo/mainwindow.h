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
    QWidget *groupBox;

    void openContent();
    void initGlobals();
    void deleteGlobals();
    void createAppSubdir(QString subdirName);


protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionFreehand_triggered();
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
