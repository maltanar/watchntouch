/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QDeclarativeView>
#include <QScrollArea>
#include <QHash>
#include <QTimer>
#include "presentationdisplaytask.h"
#include "videodisplaytask.h"
#include "webpagedisplaytask.h"
#include "sketchingtask.h"
#include "collaborativedrawingtask.h"

#include "screencasting.h"
#include "screenshot.h"


#include "qmlmenulayer.h"

#define COLLABORATIVE_DRAWING_TASK_ID   "collaborative_drawing"

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
    QWidget *m_currentTaskContainer;
    Ui::MainWindow *ui;
    QString m_selectedContent;
    QHash<QString, ContentDisplayTask *> m_tasks;
    ContentDisplayTask * m_activeTask;
    QString m_activeTaskID;
    Screenshot m_screenshot;
    Screencasting m_screencast;
    QTimer m_taskManagerUpdateTimer;
    int m_taskScrollerTaskType;

    void initGlobals();
    void deleteGlobals();
    void createAppSubdir(QString subdirName);
    void connectMainMenuSignals();
    void clearCache();

    QString openContent(ContentType type);
    void openPresentation();
    void openWebPage();
    void openMultimedia();

    void setActiveTask(QString taskID);

    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private slots:
    void mainMenuShowHide(bool newStatus);
    void exitPressed();
    void recordPressed(bool newStatus);
    void notificationsPressed();
    void presentationPressed();
    void webPressed();
    void multimediaPressed();
    void sketchPressed();
    void collaborationPressed();
    void fullscreenStateChange();
    void taskManagerShowHide();
    void newTask(int id);
    void receiveScreenshot(QPixmap img);
    void updateTaskScroller(int taskType);
    void switchToTask(QString taskID);
    void killTask(QString taskID);

    void MainGui_alignTaskScrollerToSelectedTask(QString taskID);
    void MainGui_addToTaskManagerScroller(QString pathOfTheImage, QString taskId);
    void MainGui_clearTaskManagerScroller();
    void MainGui_taskManagerShowHide(bool show);
    bool MainGui_isTaskScrollerVisible();

    void taskManagerUpdateTimeout();

};

#endif // MAINWINDOW_H
