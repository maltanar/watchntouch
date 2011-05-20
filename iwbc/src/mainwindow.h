#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QDeclarativeView>
#include <QScrollArea>
#include "presentationdisplaywidget.h"
#include "sketchingwidget.h"
#include "annotationwidget.h"
#include "contextmenu.h"
#include "screencasting.h"

#include "QjtMouseGesture.h"
#include "screenshot.h"

#include "videounderlay.h"
#include "videocontrolpanel.h"

#include "webpagedisplaywidget.h"
#include "webcontrolpanel.h"

#include "qmlmenulayer.h"

namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    QStackedWidget *m_widgetStack;
    QMLMenuLayer *m_qmlMenu;
    QScrollArea *m_currentTaskContainer;
    Ui::MainWindow *ui;

    void initGlobals();
    void deleteGlobals();
    void createAppSubdir(QString subdirName);

    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private slots:
    void mainMenuShowHide(bool newStatus);


};

#endif // MAINWINDOW_H
