#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QDeclarativeView>
#include <QScrollArea>
#include <QHash>
#include "presentationdisplaytask.h"

#include "screencasting.h"
#include "screenshot.h"


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
    QString m_selectedContent;
    QHash<QString, ContentDisplayTask *> m_tasks;

    Screencasting m_screencast;

    void initGlobals();
    void deleteGlobals();
    void createAppSubdir(QString subdirName);
    void connectMainMenuSignals();

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


};

#endif // MAINWINDOW_H
