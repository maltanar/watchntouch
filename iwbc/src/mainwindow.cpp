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
    // create the central stacked layout
    QVBoxLayout * centralStretcher = new QVBoxLayout();
    QStackedLayout * centralStack = new QStackedLayout();
    // create the QML main menu
    m_qmlMenu = new QMLMenuLayer(this);
    m_currentTaskContainer = new QScrollArea(this);
    centralStack->addWidget(m_currentTaskContainer);
    centralStack->addWidget(m_qmlMenu);
    centralStack->setStackingMode(QStackedLayout::StackAll);
    centralStretcher->addLayout(centralStack);

    m_currentTaskContainer->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    connectMainMenuSignals();

    //m_qmlMenu->setStyleSheet("border: 2px solid red");
    //m_currentTaskContainer->setStyleSheet("border: 3px solid yellow; background: red");
    ui->theCentralWidget->setLayout(centralStretcher);

}

void MainWindow::connectMainMenuSignals()
{
    // connect the signals emitted from the QML main menu to the slots we have here
    connect(m_qmlMenu->rootObject(), SIGNAL(mainMenuShowHide(bool)), this, SLOT(mainMenuShowHide(bool)));
    connect(m_qmlMenu->rootObject(), SIGNAL(exitPressed()), this, SLOT(exitPressed()));
    connect(m_qmlMenu->rootObject(), SIGNAL(recordPressed(bool)), this, SLOT(recordPressed(bool)));
    connect(m_qmlMenu->rootObject(), SIGNAL(notificationsPressed()), this, SLOT(notificationsPressed()));
    connect(m_qmlMenu->rootObject(), SIGNAL(presentationPressed()), this, SLOT(presentationPressed()));
    connect(m_qmlMenu->rootObject(), SIGNAL(webPressed()), this, SLOT(webPressed()));
    connect(m_qmlMenu->rootObject(), SIGNAL(multimediaPressed()), this, SLOT(multimediaPressed()));
    connect(m_qmlMenu->rootObject(), SIGNAL(sketchPressed()), this, SLOT(sketchPressed()));
    // TODO remove adjustInteractiveHeight code
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
    // do an empty content change to save any modified items
    //draw->contentChanged("");
    //videoDraw->contentChanged("");
    //webDraw->contentChanged("");
    // TODO IMPORTANT save all active tasks' changes!
    if(displayYesNoMessage("Are you sure you want to exit?", "Exiting Watch and Touch")) {
        recentlyUsed->writeToStorage();
        bookmarkList->writeToStorage();
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
        }
        // TODO let the menu know that the active task changed
       m_currentTaskContainer->setWidget(theTask);
       m_activeTask = theTask;
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
        // load the content
        newTask->getContentDisplay()->selectContent(m_selectedContent);
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
    SketchingTask * newTask;
    QString newTaskID;

    m_selectedContent = openContent(CONTENTTYPE_SKETCH);

    if(m_selectedContent != "") {
        // create and insert new task into the list of active tasks
        newTask = new SketchingTask(this);
        // set the task panel as the QML menu root object
        newTask->setPanel(m_qmlMenu->rootObject());
        newTaskID = "sketch_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        qWarning() << "new task identifier:" << newTaskID;
        m_tasks.insert(newTaskID, newTask);
        // set this as the active task
        setActiveTask(newTaskID);

        if(m_selectedContent == "$newsketch$") {
            // TODO IMPORTANT add to recently used list
            newTask->newSketch("sketch - " + QDateTime::currentDateTime().toString());
        }

    }
}


