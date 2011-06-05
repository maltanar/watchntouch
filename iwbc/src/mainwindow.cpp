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

#include "appglobals.h"

RecentlyUsed *recentlyUsed;
GoogleDocsAccess *googleDocsAccess;
EventGenerator *eventGenerator;

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
}

void MainWindow::mainMenuShowHide(bool newStatus)
{
    if(newStatus) {
        qWarning() << "menu now visible";
    } else {
        qWarning() << "menu now hidden";
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
    recentlyUsed->writeToStorage();
    event->accept();
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
        }
        // TODO let the menu know that the active task changed
       m_currentTaskContainer->setWidget(theTask);
       m_activeTask = theTask;
       // let the active widget know it's been activated
       m_activeTask->activate();
       // TODO RESIZING erronous logic here?
       theTask->resize(m_currentTaskContainer->size() - QSize(5,5));
    }
}

void MainWindow::openWebPage()
{
    m_selectedContent = openContent(CONTENTTYPE_WEBPAGE);
}

void MainWindow::openMultimedia()
{
    VideoDisplayTask * newTask;
    QString newTaskID;

    m_selectedContent = openContent(CONTENTTYPE_VIDEO);

    if(m_selectedContent != "") {
        // create and insert new task into the list of active tasks
        newTask = new VideoDisplayTask(this);
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

    /*if(selectedContent == "$screenshot$") {
        openScreenshot();
    } else if (selectedContent == "$newsketch$") {
        openNewSketch();
    } else if (selectedContent == "$existingsketch$") {
        openExistingSketch();
    } else if (selectedContent == "$webpage$") {
        // TODO move to own function
        widgetStack->setCurrentIndex(WEBPAGE_ANNOTATION);
        widgetStack->resize(ui->scrollArea->size()-QSize(10,10));
        groupBoxForWeb->resize(ui->scrollArea->size()-QSize(10,10));
        webDraw->attachToContentDisplay(webDisplay);
        //webDisplay->selectContent("");
        webDisplay->raise();
        webDraw->raise();
    }
    else {
        // TODO check content type before loading?
        // TODO the code below won't apply once we have fit to height/width options
        // set desired image size to a bit smaller than the scroll area size
        if(selectedContent.endsWith("pdf") || selectedContent.endsWith("ppt") || selectedContent.endsWith("odp") ) {

            widgetStack->setCurrentIndex(PRESENTATION_ANNOTATION);

            display->setDesiredSize(ui->scrollArea->size()-QSize(10,10));
            draw->attachToContentDisplay(display);  // TODO bunu bir presentation actıktan sonra yapmak lazım ?
            display->selectContent(csel.getSelectedContent());

            widgetStack->resize(display->getContentSize());
            if(widgetStack->width() < 800)
                widgetStack->resize(800, widgetStack->height());
            draw->raise();
            //qmlMenu->raise();

        } else if(selectedContent.endsWith("mp4") || selectedContent.endsWith("avi") || selectedContent.endsWith("flv")) {

            widgetStack->setCurrentIndex(VIDEO_ANNOTATION);

            widgetStack->resize(ui->scrollArea->size()-QSize(10,10));
            videoDraw->attachToContentDisplay(videoPlayer);
            videoPlayer->selectContent(selectedContent);
            videoPlayer->raise();
            videoDraw->raise();

        } else if(selectedContent.endsWith("html") || selectedContent.endsWith("htm") || selectedContent.startsWith("www") || selectedContent.startsWith("http")) {
            // TODO webpage resize problems
            // TODO find a way to scroll the webpage
            widgetStack->setCurrentIndex(WEBPAGE_ANNOTATION);

            widgetStack->resize(ui->scrollArea->size()-QSize(10,10));
            groupBoxForWeb->resize(ui->scrollArea->size()-QSize(10,10));
            //webCanvas->resize(ui->scrollArea->size()-QSize(50,10));
            webDraw->attachToContentDisplay(webDisplay);
            webDisplay->selectContent(selectedContent);
            webDisplay->raise();
            webDraw->raise();
        }
    }*/
}


void MainWindow::sketchPressed()
{
    m_selectedContent = openContent(CONTENTTYPE_SKETCH);
}
