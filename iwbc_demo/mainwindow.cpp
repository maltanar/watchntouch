#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDir>
#include "contentselector.h"
#include "googledocsaccess.h"

RecentlyUsed *recentlyUsed;
GoogleDocsAccess *googleDocsAccess;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initGlobals();

    display = new PresentationDisplayWidget(this);
    draw = new AnnotationWidget(this);

    groupBox = new QWidget(this);

    QStackedLayout *layout = new QStackedLayout();

    layout->addWidget(display);
    layout->addWidget(draw);
    layout->setStackingMode(QStackedLayout::StackAll);
    layout->setAlignment(display, Qt::AlignHCenter);
    layout->setAlignment(draw, Qt::AlignHCenter);
    groupBox->setLayout(layout);

    ui->scrollArea->setWidget(groupBox);

    connect(ui->actionNext, SIGNAL(triggered()), display, SLOT(gotoNextSlide()));
    connect(ui->actionPrevious, SIGNAL(triggered()), display, SLOT(gotoPrevSlide()));

    connect(ui->actionUndo, SIGNAL(triggered()), draw->getDrawingData()->getUndoStack(),SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered()), draw->getDrawingData()->getUndoStack(),SLOT(redo()));

    draw->attachToContentDisplay(display);
    draw->setStyleSheet("background: transparent");
}



MainWindow::~MainWindow()
{
    deleteGlobals();
    delete ui;
}

void MainWindow::openContent()
{
    ContentSelector csel;
    if(csel.exec() != QDialog::Accepted)
        // no content selected, do nothing
        return;

    // TODO check content type before loading?
    // TODO the code below won't apply once we have fit to height/width options
    // set desired image size to a bit smaller than the scroll area size
    display->setDesiredSize(ui->scrollArea->size()-QSize(10,10));
    display->selectContent(csel.getSelectedContent());
    groupBox->resize(display->getContentSize());
    draw->raise();
}

void MainWindow::on_actionOpen_triggered()
{
    openContent();
}

void MainWindow::initGlobals()
{
    // create the config directory
    createAppSubdir(CONFIG_DIR);
    // create the cache directory
    createAppSubdir(CACHE_DIR);

    recentlyUsed = new RecentlyUsed();
    googleDocsAccess = new GoogleDocsAccess();
}

void MainWindow::deleteGlobals()
{
    delete recentlyUsed; recentlyUsed = NULL;
    delete googleDocsAccess; googleDocsAccess = NULL;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // do an empty content change to save any modified items
    draw->contentChanged("");
    event->accept();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionFreehand_triggered()
{
    draw->setDrawingMode(DRAWINGMODE_FREEHAND);
}

void MainWindow::createAppSubdir(QString subdirName)
{
    QString dirPath= qApp->applicationDirPath() + "/" + subdirName;

    QDir dir(dirPath);
    // check if annotations directory exists
    if(!dir.exists()) {
        // directory does not exist, create it
        dir.mkdir(dirPath);
    }
}
