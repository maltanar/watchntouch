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
    qWarning() << "record pressed" << newStatus;
}

void MainWindow::notificationsPressed()
{
    qWarning() << "notifications pressed";
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
    recentlyUsed->writeToStorage();
    // TODO BUG recent items list is lost on closing the app?
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
