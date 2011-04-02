#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "presentationdisplaywidget.h"
#include "sketchingwidget.h"
#include "annotationwidget.h"
#include "contextmenu.h"
#include "screencasting.h"

#include "QjtMouseGesture.h"
#include "screenshot.h"

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
    SketchingWidget *drawSketch;
    SketchingWidget *drawScreenshot;
    AnnotationWidget *draw;
    QWidget *groupBoxForPresentation;
    QStackedWidget *widgetStack;
    QWidget *groupBoxForSketching;
    QLabel *screenshotOrBlankImage;

    ContextMenu *contextMenu;
    QjtMouseGesture *g;
    DirectionList dl;
    Screencasting sc;
    Screenshot * scrnsht;

    void initGlobals();
    void deleteGlobals();
    void createAppSubdir(QString subdirName);


protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionScreencasting_triggered();
    void on_actionFreehand_triggered();
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void showContextMenu(QPoint p);

public slots:
    void openContent();
    void saveContent();
    void openNewSketch();
    void openExistingSketch();
    void openScreenshot();
    void getScreenshot();

signals:
    void contentChanged(QString newContent);
    void contextChanged(QString newContext);
};

#endif // MAINWINDOW_H
