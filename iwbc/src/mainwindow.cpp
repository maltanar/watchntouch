#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDir>
#include <QImageWriter>
#include <QPrinter>
#include <QFile>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QDeclarativeView>
#include <QResizeEvent>
#include <QGraphicsObject>
#include "contentselector.h"
#include "googledocsaccess.h"
#include "eventgenerator.h"
#include "bookmarklist.h"

#include <QMetaMethod>

#include "appglobals.h"

RecentlyUsed *recentlyUsed;
GoogleDocsAccess *googleDocsAccess;
EventGenerator *eventGenerator;
BookmarkList *bookmarkList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // setup the global variables and create the subdirs (if needed)
    initGlobals();
    m_activeTask = NULL;
    m_taskScrollerTaskType = -1;
    // create the central stacked layout
    QVBoxLayout * centralStretcher = new QVBoxLayout();
    QStackedLayout * centralStack = new QStackedLayout();
    // create the QML main menu
    m_qmlMenu = new QMLMenuLayer(this);
    m_currentTaskContainer = new QWidget(this);
    centralStretcher->setContentsMargins(1,1,1,1);
    centralStack->setContentsMargins(1,1,1,1);
    centralStack->addWidget(m_currentTaskContainer);
    centralStack->addWidget(m_qmlMenu);
    centralStack->setStackingMode(QStackedLayout::StackAll);
    centralStretcher->addLayout(centralStack);

    //m_currentTaskContainer->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    connectMainMenuSignals();

    //m_qmlMenu->setStyleSheet("border: 2px solid red");
    //m_currentTaskContainer->setStyleSheet("border: 3px solid yellow; background: red");
    ui->theCentralWidget->setLayout(centralStretcher);

    connect(&m_screenshot, SIGNAL(acquiredScreenshot(QPixmap)), this, SLOT(receiveScreenshot(QPixmap)));
}

void MainWindow::taskManagerShowHide()
{
    //QMetaObject::invokeMethod(m_qmlMenu->rootObject(), "taskManagerShowHide");
}

void MainWindow::connectMainMenuSignals()
{
    // connect the signals emitted from the QML main menu to the slots we have here
    connect(m_qmlMenu->rootObject(), SIGNAL(mainMenuShowHide(bool)), this, SLOT(mainMenuShowHide(bool)));
    connect(m_qmlMenu->rootObject(), SIGNAL(exitPressed()), this, SLOT(exitPressed()));
    connect(m_qmlMenu->rootObject(), SIGNAL(recordPressed(bool)), this, SLOT(recordPressed(bool)));
    connect(m_qmlMenu->rootObject(), SIGNAL(notificationsPressed()), this, SLOT(notificationsPressed()));
    //connect(m_qmlMenu->rootObject(), SIGNAL(presentationPressed()), this, SLOT(taskManagerShowHide()));
    //connect(m_qmlMenu->rootObject(), SIGNAL(webPressed()), this, SLOT(taskManagerShowHide()));
    //connect(m_qmlMenu->rootObject(), SIGNAL(multimediaPressed()), this, SLOT(taskManagerShowHide()));
    connect(m_qmlMenu->rootObject(), SIGNAL(sketchPressed()), this, SLOT(sketchPressed()));
    connect(m_qmlMenu->rootObject(), SIGNAL(fullscreenStateChange()), this, SLOT(fullscreenStateChange()));
    connect(m_qmlMenu->rootObject(), SIGNAL(newTask(int)), this, SLOT(newTask(int)));
    connect(m_qmlMenu->rootObject(), SIGNAL(openTaskManager(int)), this, SLOT(updateTaskScroller(int)));
    connect(m_qmlMenu->rootObject(), SIGNAL(switchToTask(QString)), this, SLOT(switchToTask(QString)));
}

void MainWindow::fullscreenStateChange()
{
    if(isFullScreen())
        showMaximized();
    else
        showFullScreen();
}

void MainWindow::mainMenuShowHide(bool newStatus)
{
    if(newStatus) {
        qWarning() << "menu now visible";
        //m_qmlMenu->setMaskHeightFromBottom(110);
    } else {
        qWarning() << "menu now hidden";
        //m_qmlMenu->setMaskHeightFromBottom(35);
    }
}

void MainWindow::recordPressed(bool newStatus)
{
    if(newStatus)
        m_screencast.startScreencasting();
    else
        m_screencast.stopScreencasting();
}

void MainWindow::notificationsPressed()
{
    qWarning() << "notifications pressed";
}

void MainWindow::presentationPressed()
{
    openPresentation();
}

void MainWindow::webPressed()
{
    openWebPage();
}

void MainWindow::multimediaPressed()
{
    openMultimedia();
}

MainWindow::~MainWindow()
{
    deleteGlobals();
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);

    m_qmlMenu->resize(m_currentTaskContainer->size());
}

void MainWindow::initGlobals()
{
    // create the config directory
    createAppSubdir(CONFIG_DIR);
    // create the cache directory
    createAppSubdir(CACHE_DIR);
    // create the tools directory
    createAppSubdir(TOOLS_DIR);
    // extract the document converter script
    if(!QFile::exists(DOC_CONVERTER_PATH)) {
        QFile docConverter(DOC_CONVERTER_RES);
        docConverter.open(QFile::ReadOnly);
        docConverter.copy(DOC_CONVERTER_PATH);
        docConverter.close();
    }
    // create the globally used classes
    recentlyUsed = new RecentlyUsed();
    googleDocsAccess = new GoogleDocsAccess();
    eventGenerator = new EventGenerator();
    bookmarkList = new BookmarkList();
}

void MainWindow::deleteGlobals()
{
    delete recentlyUsed; recentlyUsed = NULL;
    delete googleDocsAccess; googleDocsAccess = NULL;
    delete eventGenerator; eventGenerator = NULL;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // TODO add save function to drawing
    // TODO clear some of the cache here?
    // do an empty content change to save any modified items
    //draw->contentChanged("");
    //videoDraw->contentChanged("");
    //webDraw->contentChanged("");
    // TODO IMPORTANT save all active tasks' changes!
    if(displayYesNoMessage("Are you sure you want to exit?", "Exiting Watch and Touch")) {
        recentlyUsed->writeToStorage();
        bookmarkList->writeToStorage();
        if(m_activeTask) {
            m_activeTask->deactivate();
        }
        event->accept();
    } else
        event->ignore();
}

void MainWindow::createAppSubdir(QString subdirName)
{
    QDir dir(subdirName);
    // check if annotations directory exists
    if(!dir.exists()) {
        // directory does not exist, create it
        dir.mkpath(subdirName);
    }
}

void MainWindow::exitPressed()
{
    close();
}

void MainWindow::openPresentation()
{
    PresentationDisplayTask * newTask;
    QString newTaskID;
    m_selectedContent = openContent(CONTENTTYPE_PRESENTATION);

    if(m_selectedContent != "") {
        // create and insert new task into the list of active tasks
        newTask = new PresentationDisplayTask(this);
        // set the task panel as the QML menu root object
        newTask->setPanel(m_qmlMenu->rootObject());
        newTaskID = "presentation_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        qWarning() << "new task identifier:" << newTaskID;
        m_tasks.insert(newTaskID, newTask);
        // set this as the active task
        setActiveTask(newTaskID);
        // load the content
        newTask->getContentDisplay()->selectContent(m_selectedContent);
    }
}

void MainWindow::setActiveTask(QString taskID)
{
    ContentDisplayTask * theTask = m_tasks.value(taskID, NULL);

    if(theTask) {
        if(m_activeTask) {
            // deactivate previous task
            m_activeTask->deactivate();
            // disconnect mouse event signals and slots
            disconnect(m_qmlMenu, SIGNAL(mousePressSignal(QPoint,int)), m_activeTask, SLOT(mousePress(QPoint,int,int)));
            disconnect(m_qmlMenu, SIGNAL(mouseMoveSignal(QPoint,int)), m_activeTask, SLOT(mouseMove(QPoint,int,int)));
            disconnect(m_qmlMenu, SIGNAL(mouseReleaseSignal(QPoint,int)), m_activeTask, SLOT(mouseRelease(QPoint,int,int)));
            m_activeTask->hide();
        }
        // TODO let the menu know that the active task changed
       //m_currentTaskContainer->setWidget(theTask);

       m_activeTask = theTask;
       m_activeTask->setParent(m_currentTaskContainer);
       m_activeTask->show();
       // let the active widget know it's been activated
       m_activeTask->activate();
        // connect mouse event signals and slots
       connect(m_qmlMenu, SIGNAL(mousePressSignal(QPoint,int,int)), m_activeTask, SLOT(mousePress(QPoint,int,int)));
       connect(m_qmlMenu, SIGNAL(mouseMoveSignal(QPoint,int,int)), m_activeTask, SLOT(mouseMove(QPoint,int,int)));
       connect(m_qmlMenu, SIGNAL(mouseReleaseSignal(QPoint,int,int)), m_activeTask, SLOT(mouseRelease(QPoint,int,int)));
       // TODO RESIZING erronous logic here?
       theTask->resize(m_currentTaskContainer->size() - QSize(5,5));
    }
}

void MainWindow::openWebPage()
{
    WebPageDisplayTask * newTask;
    QString newTaskID;

    m_selectedContent = openContent(CONTENTTYPE_WEBPAGE);

    if(m_selectedContent != "") {
        // create and insert new task into the list of active tasks
        newTask = new WebPageDisplayTask(this);
        // set the task panel as the QML menu root object
        newTask->setPanel(m_qmlMenu->rootObject());
        newTaskID = "webpage_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        qWarning() << "new task identifier:" << newTaskID;
        m_tasks.insert(newTaskID, newTask);
        // set this as the active task
        setActiveTask(newTaskID);
        if(m_selectedContent != "$webpage$") {
            // load the content
            newTask->getContentDisplay()->selectContent(m_selectedContent);
        }
    }
}

void MainWindow::openMultimedia()
{
    VideoDisplayTask * newTask;
    QString newTaskID;

    m_selectedContent = openContent(CONTENTTYPE_VIDEO);

    if(m_selectedContent != "") {
        // create and insert new task into the list of active tasks
        newTask = new VideoDisplayTask(this);
        // set the task panel as the QML menu root object
        newTask->setPanel(m_qmlMenu->rootObject());
        newTaskID = "video_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        qWarning() << "new task identifier:" << newTaskID;
        m_tasks.insert(newTaskID, newTask);
        // set this as the active task
        setActiveTask(newTaskID);
        // load the content
        newTask->getContentDisplay()->selectContent(m_selectedContent);
    }
}

QString MainWindow::openContent(ContentType type)
{
    ContentSelector csel(type);
    if(csel.exec() != QDialog::Accepted)
        // no content selected, do nothing
        return "";

    //widgetStack->setVisible(true);

    // TODO implement context saving when we switch from one content type to the other!
    // e.g presentation -> another presentation saves the annotations, but presentation -> video
    // just hides the presentation and doesn't save the changes that was made on the presentation
    // it is still saved when we close the app but we shouldn't rely on that

    return csel.getSelectedContent();
}


void MainWindow::sketchPressed()
{

}

void MainWindow::receiveScreenshot(QPixmap img)
{
    ((SketchingTask*)m_activeTask)->sketchFromImage("screenshot- " + QDateTime::currentDateTime().toString(), img);
}

void MainWindow::newTask(int id)
{
    ContentDisplayTask * newTask;
    QString newTaskID;

    switch(id) {
    case TASK_PRESENTATION:
        m_selectedContent = openContent(CONTENTTYPE_PRESENTATION);

        if(m_selectedContent != "") {
            // create and insert new task into the list of active tasks
            newTask = new PresentationDisplayTask(this);
            newTaskID = "presentation_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        }
        break;
    case TASK_SKETCHING:
        m_selectedContent = openContent(CONTENTTYPE_SKETCH);

        if(m_selectedContent != "") {
            // create and insert new task into the list of active tasks
            newTask = new SketchingTask(this);
            newTaskID = "sketch_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        }
        break;
    case TASK_WEBPAGE:
        m_selectedContent = openContent(CONTENTTYPE_WEBPAGE);

        if(m_selectedContent != "") {
            // create and insert new task into the list of active tasks
            newTask = new WebPageDisplayTask(this);
            newTaskID = "webpage_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        }
        break;
    case TASK_VIDEO:
        m_selectedContent = openContent(CONTENTTYPE_VIDEO);

        if(m_selectedContent != "") {
            // create and insert new task into the list of active tasks
            newTask = new VideoDisplayTask(this);
            newTaskID = "video_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        }
        break;
    default:
        qWarning() << "task handler for" << id << "is not yet implemented!";
    }

    if(newTaskID != "") {
        qWarning() << "new task identifier:" << newTaskID;

        // set the task panel as the QML menu root object
        newTask->setPanel(m_qmlMenu->rootObject());

        m_tasks.insert(newTaskID, newTask);
        // set this as the active task
        setActiveTask(newTaskID);

        if(m_selectedContent == "$newsketch$") {
            // TODO IMPORTANT add to recently used list
            ((SketchingTask*)newTask)->newSketch("sketch - " + QDateTime::currentDateTime().toString());
        } else if(m_selectedContent == "$screenshot$") {
            m_screenshot.show();
        } else if(m_selectedContent != "") {
            newTask->getContentDisplay()->selectContent(m_selectedContent);
        }
    }
}

void MainWindow::MainGui_alignTaskScrollerToSelectedTask(int index)
{
    QVariant iIndex = QVariant::fromValue(index);
    QMetaObject::invokeMethod(m_qmlMenu->rootObject(), "alignTaskScrollerToSelectedTask", Q_ARG(QVariant, iIndex));
}

void MainWindow::MainGui_addToTaskManagerScroller(QString pathOfTheImage, QString taskId)
{
    QVariant sTaskID = QVariant::fromValue(taskId);
    QVariant sPath = QVariant::fromValue("file://"+pathOfTheImage);
    QMetaObject::invokeMethod(m_qmlMenu->rootObject(), "addToTaskManagerScroller",
                              Q_ARG(QVariant, sPath),
                              Q_ARG(QVariant, sTaskID));
}

void MainWindow::MainGui_clearTaskManagerScroller()
{
    QMetaObject::invokeMethod(m_qmlMenu->rootObject(), "clearTaskManagerScroller");
}

void MainWindow::updateTaskScroller(int taskType)
{
    qWarning() << "updateTaskScroller" << taskType;
    // clear the task list..
    MainGui_clearTaskManagerScroller();
    // .. and insert task instances of the desired type
    ContentDisplayTask * currentTask;
    QList<QString> taskIDs = m_tasks.keys();
    QString imgFileName, currentTaskID;
    for(int i=0; i < taskIDs.count(); i++) {
        currentTaskID = taskIDs.at(i);
        currentTask = m_tasks.value(currentTaskID, NULL);
        if(currentTask->getTaskType() == taskType) {
            qWarning() << taskIDs.at(i) << "is of desired type";
            imgFileName = CACHE_DIR + "/" + currentTaskID + ".png";
            currentTask->getTaskScreenshot().save(imgFileName);
            MainGui_addToTaskManagerScroller(imgFileName, currentTaskID);
        }
    }
}

void MainWindow::switchToTask(QString taskID)
{
    setActiveTask(taskID);
}
