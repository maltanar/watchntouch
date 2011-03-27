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
#include <QTimer>
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

    initGlobals();

    display = new PresentationDisplayWidget(this);
    draw = new AnnotationWidget(this);
    contextMenu = new ContextMenu(this);
    scrnsht = new Screenshot(this);
    scrnsht->hide();
    contextMenu->hide();

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
    //draw->setStyleSheet("background: transparent");

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));
    connect(contextMenu, SIGNAL(undo()), draw->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(contextMenu, SIGNAL(redo()), draw->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(contextMenu, SIGNAL(toolSelected(DrawingMode)), draw, SLOT(setDrawingMode(DrawingMode)));
    connect(contextMenu, SIGNAL(open()), this, SLOT(openContent()));
    connect(contextMenu, SIGNAL(save()), this, SLOT(saveContent()));
    connect(contextMenu, SIGNAL(sketch()), this, SLOT(openSketch()));
    connect(contextMenu, SIGNAL(screenshot()), this, SLOT(openScreenshot()));
    connect(contextMenu, SIGNAL(colorSelected(QColor)), draw, SLOT(setDrawingColor(QColor)));
    connect(contextMenu, SIGNAL(penWidthIncrease()), draw, SLOT(increasePenWidth()));
    connect(contextMenu, SIGNAL(penWidthDecrease()), draw, SLOT(decreasePenWidth()));
    connect(scrnsht,SIGNAL(goBack()),this,SLOT(getScreenshot()));

    dl << Left;
    g = new QjtMouseGesture( dl, this);
    eventGenerator->addGesture( g );
    display->connect( g, SIGNAL(gestured()), SLOT(gotoPrevSlide()) );

    dl.clear();
    dl << Right;
    g = new QjtMouseGesture( dl, this );
    eventGenerator->addGesture( g );
    display->connect( g, SIGNAL(gestured()), SLOT(gotoNextSlide()) );

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

void MainWindow::openSketch()
{
    QString sketchDirPath = SKETCH_DIR;
    QDir sketchDir(sketchDirPath);
    QString path;
    path.append(sketchDirPath).append("/sketch.pdf");
    if(!sketchDir.exists()) {
        sketchDir.mkdir(sketchDirPath);

        QFile sketch_file(path);
        sketch_file.open(QIODevice::ReadWrite | QIODevice::Text);

        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(path);
        printer.setFullPage(true);
        printer.setOrientation(printer.Landscape);

        QPainter painter;
        if (! painter.begin(&printer)) { // failed to open file
            qWarning("failed to open file, is it writable?");
            return;
        }
        painter.end();
    }
    display->setDesiredSize(ui->scrollArea->size()-QSize(10,10));
    display->selectContent(path);
    groupBox->resize(display->getContentSize());
    draw->raise();
}

void MainWindow::openScreenshot()
{
    hide();
    scrnsht->show();
    //QTimer::singleShot(500, this, SLOT(getScreenshot()));
}

void MainWindow::getScreenshot()
{
    scrnsht->hide();
    usleep(5000);

    QPixmap originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    showFullScreen();

    QString format = "png";
    QString screenshotDirPath = SCREENSHOT_DIR;
    QDir screenshotDir(screenshotDirPath);
    if(!screenshotDir.exists()) {
        screenshotDir.mkdir(screenshotDirPath);
    }

    QString fileName = screenshotDirPath;
    fileName = fileName.append("/screenshot.png");
    if (!fileName.isEmpty())
       originalPixmap.save(fileName, format.toAscii());

    display->setDesiredSize(ui->scrollArea->size()-QSize(10,10));
    display->selectContent("screenshot.scrn");      // just to communicate internally, no such a file extension !
    groupBox->resize(display->getContentSize());
    draw->raise();

}


void MainWindow::saveContent()
{
    // TODO add save function to drawing
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
    QDir dir(subdirName);
    // check if annotations directory exists
    if(!dir.exists()) {
        // directory does not exist, create it
        dir.mkpath(subdirName);
    }
}

void MainWindow::showContextMenu(QPoint p)
{
    qWarning() << "caylar da caymis hani...o zaman context menu acalim. hoppala yavrum kokakola.";
    contextMenu->move(p - QPoint(contextMenu->width()/2, contextMenu->height()/2));
    contextMenu->show();
}

void MainWindow::on_actionScreencasting_triggered()
{
    if(!sc.isRunning())
        sc.startScreencasting();
    else
        sc.stopScreencasting();
}
