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

    groupBoxForPresentation = new QWidget(this);

    QStackedLayout *layout = new QStackedLayout();

    layout->addWidget(display);
    layout->addWidget(draw);
    layout->setStackingMode(QStackedLayout::StackAll);
    layout->setAlignment(display, Qt::AlignHCenter);
    layout->setAlignment(draw, Qt::AlignHCenter);

    groupBoxForPresentation->setLayout(layout);

    // video playing and annotation components *********************
    groupBoxForVideo = new QWidget(this);
    videoCanvas = new QWidget(this);

    videoPlayer = new VideoUnderlay(videoCanvas);
    videoDraw = new AnnotationWidget(videoCanvas);

    videoPanel = new VideoControlPanel(groupBoxForVideo);

    QStackedLayout *layoutForCanvas = new QStackedLayout();
    layoutForCanvas->addWidget(videoPlayer);
    layoutForCanvas->addWidget(videoDraw);
    layoutForCanvas->setStackingMode(QStackedLayout::StackAll);
    layoutForCanvas->setAlignment(display, Qt::AlignHCenter);
    layoutForCanvas->setAlignment(draw, Qt::AlignHCenter);

    videoCanvas->setLayout(layoutForCanvas);

    QVBoxLayout *layoutForVideo = new QVBoxLayout();
    layoutForVideo->addWidget(videoCanvas);
    layoutForVideo->addWidget(videoPanel);

    groupBoxForVideo->setLayout(layoutForVideo);

    // TODO fix video group resize issues

    // connect signals and slots for video

    connect(videoPanel, SIGNAL(playClicked()), videoPlayer, SLOT(play()));
    connect(videoPanel, SIGNAL(pauseClicked()), videoPlayer, SLOT(pause()));
    connect(videoPanel, SIGNAL(timelineChanged(float)), videoPlayer, SLOT(seekTo(float)));
    connect(videoPlayer, SIGNAL(mediaLengthUpdate(float)), videoPanel, SLOT(mediaLengthChanged(float)));
    connect(videoPlayer, SIGNAL(timelineUpdate(float)), videoPanel, SLOT(mediaPosChanged(float)));


    // video playing and annotation components *********************

    // webpage display and annotation components *********************
    groupBoxForWeb = new QWidget(this);
    webCanvas = new QWidget(this);

    webDisplay = new WebpageDisplayWidget(webCanvas);
    webDraw = new AnnotationWidget(webCanvas);

    webControlPanel = new WebControlPanel(groupBoxForWeb);

    QStackedLayout *layoutForWebCanvas = new QStackedLayout();
    layoutForWebCanvas->addWidget(webDisplay);
    layoutForWebCanvas->addWidget(webDraw);
    layoutForWebCanvas->setStackingMode(QStackedLayout::StackAll);
    layoutForWebCanvas->setAlignment(display, Qt::AlignHCenter);
    layoutForWebCanvas->setAlignment(draw, Qt::AlignHCenter);

    webCanvas->setLayout(layoutForWebCanvas);

    QVBoxLayout *layoutForWeb = new QVBoxLayout();
    layoutForWeb->addWidget(webCanvas);
    layoutForWeb->addWidget(webControlPanel);

    groupBoxForWeb->setLayout(layoutForWeb);

    // connect signals and slots for web

    connect(webControlPanel, SIGNAL(locationChanged(QUrl)), webDisplay, SLOT(loadWebPage(QUrl)));
    connect(webDisplay, SIGNAL(webPageLoadStarted()), webControlPanel, SLOT(loadStarted()));
    connect(webDisplay, SIGNAL(webPageLoadProgress(int)), webControlPanel, SLOT(loadProgress(int)));
    connect(webDisplay, SIGNAL(webPageLoadFinished(bool)), webControlPanel, SLOT(loadFinished(bool)));
    connect(webDisplay, SIGNAL(webPageUrlChanged(QUrl)), webControlPanel, SLOT(loadedPageChanged(QUrl)));


    // webpage display and annotation components *********************


    /************** Sketching component init begins ***********************/

    drawSketch = new SketchingWidget(this);
    drawScreenshot = new SketchingWidget(this);
    screenshotOrBlankImage = new QLabel(this);

    groupBoxForSketching = new QWidget(this);
    QStackedLayout *layoutForSketching = new QStackedLayout();

    layoutForSketching->addWidget(drawSketch);
    layoutForSketching->addWidget(drawScreenshot);
    layoutForSketching->addWidget(screenshotOrBlankImage);
    layoutForSketching->setStackingMode(QStackedLayout::StackAll);
    layoutForSketching->setAlignment(drawSketch, Qt::AlignHCenter);
    layoutForSketching->setAlignment(screenshotOrBlankImage, Qt::AlignHCenter);

    groupBoxForSketching->setLayout(layoutForSketching);

    /************** ends ***********************/

    /************** Build the stack for sketch and presentation groupboxes ***********/

    widgetStack = new QStackedWidget(this);

    widgetStack->addWidget(groupBoxForPresentation);
    widgetStack->addWidget(groupBoxForSketching);
    widgetStack->addWidget(groupBoxForVideo);
    widgetStack->addWidget(groupBoxForWeb);

    ui->scrollArea->setWidget(widgetStack);
    widgetStack->setVisible(false);

    /************* ends ************************/

    //connect(this,SIGNAL(contentChanged(QString)), drawSketch, SLOT(contentChanged(QString)));
    //connect(this,SIGNAL(contextChanged(QString)), drawSketch, SLOT(contextChanged(QString)));
    //connect(this,SIGNAL(contentChanged(QString)), drawScreenshot, SLOT(contentChanged(QString)));
    //connect(this,SIGNAL(contextChanged(QString)), drawScreenshot, SLOT(contextChanged(QString)));

    connect(ui->actionNext, SIGNAL(triggered()), display, SLOT(gotoNextSlide()));
    connect(ui->actionPrevious, SIGNAL(triggered()), display, SLOT(gotoPrevSlide()));

    connect(ui->actionUndo, SIGNAL(triggered()), draw->getDrawingData()->getUndoStack(),SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered()), draw->getDrawingData()->getUndoStack(),SLOT(redo()));

    //draw->setStyleSheet("background: transparent");

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));
    connect(contextMenu, SIGNAL(undo()), draw->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(contextMenu, SIGNAL(redo()), draw->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(contextMenu, SIGNAL(toolSelected(DrawingMode)), draw, SLOT(setDrawingMode(DrawingMode)));
    connect(contextMenu, SIGNAL(open()), this, SLOT(openContent()));
    connect(contextMenu, SIGNAL(save()), this, SLOT(saveContent()));
    //connect(contextMenu, SIGNAL(sketch()), this, SLOT(openNewSketch())); No need I guess ! - utku
    //connect(contextMenu, SIGNAL(screenshot()), this, SLOT(openScreenshot()));
    connect(contextMenu, SIGNAL(colorSelected(QColor)), draw, SLOT(setDrawingColor(QColor)));
    connect(contextMenu, SIGNAL(penWidthIncrease()), draw, SLOT(increasePenWidth()));
    connect(contextMenu, SIGNAL(penWidthDecrease()), draw, SLOT(decreasePenWidth()));
    connect(scrnsht,SIGNAL(goBack()),this,SLOT(getScreenshot()));

    // ContextMenu bindings for SketchingWidget

    connect(contextMenu, SIGNAL(undo()), drawSketch->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(contextMenu, SIGNAL(redo()), drawSketch->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(contextMenu, SIGNAL(toolSelected(DrawingMode)), drawSketch, SLOT(setDrawingMode(DrawingMode)));
    connect(contextMenu, SIGNAL(colorSelected(QColor)), drawSketch, SLOT(setDrawingColor(QColor)));
    connect(contextMenu, SIGNAL(penWidthIncrease()), drawSketch, SLOT(increasePenWidth()));
    connect(contextMenu, SIGNAL(penWidthDecrease()), drawSketch, SLOT(decreasePenWidth()));

    connect(contextMenu, SIGNAL(undo()), drawScreenshot->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(contextMenu, SIGNAL(redo()), drawScreenshot->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(contextMenu, SIGNAL(toolSelected(DrawingMode)), drawScreenshot, SLOT(setDrawingMode(DrawingMode)));
    connect(contextMenu, SIGNAL(colorSelected(QColor)), drawScreenshot, SLOT(setDrawingColor(QColor)));
    connect(contextMenu, SIGNAL(penWidthIncrease()), drawScreenshot, SLOT(increasePenWidth()));
    connect(contextMenu, SIGNAL(penWidthDecrease()), drawScreenshot, SLOT(decreasePenWidth()));

    // ContextMenu bindings for video annotation

    connect(contextMenu, SIGNAL(undo()), videoDraw->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(contextMenu, SIGNAL(redo()), videoDraw->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(contextMenu, SIGNAL(toolSelected(DrawingMode)), videoDraw, SLOT(setDrawingMode(DrawingMode)));
    connect(contextMenu, SIGNAL(colorSelected(QColor)), videoDraw, SLOT(setDrawingColor(QColor)));
    connect(contextMenu, SIGNAL(penWidthIncrease()), videoDraw, SLOT(increasePenWidth()));
    connect(contextMenu, SIGNAL(penWidthDecrease()), videoDraw, SLOT(decreasePenWidth()));

    // ContextMenu bindings for webpage annotation

    connect(contextMenu, SIGNAL(undo()), webDraw->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(contextMenu, SIGNAL(redo()), webDraw->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(contextMenu, SIGNAL(toolSelected(DrawingMode)), webDraw, SLOT(setDrawingMode(DrawingMode)));
    connect(contextMenu, SIGNAL(colorSelected(QColor)), webDraw, SLOT(setDrawingColor(QColor)));
    connect(contextMenu, SIGNAL(penWidthIncrease()), webDraw, SLOT(increasePenWidth()));
    connect(contextMenu, SIGNAL(penWidthDecrease()), webDraw, SLOT(decreasePenWidth()));

    // Context menu bindings for screenshot

    connect(contextMenu, SIGNAL(undo()), drawScreenshot->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(contextMenu, SIGNAL(redo()), drawScreenshot->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(contextMenu, SIGNAL(toolSelected(DrawingMode)), drawScreenshot, SLOT(setDrawingMode(DrawingMode)));
    connect(contextMenu, SIGNAL(colorSelected(QColor)), drawScreenshot, SLOT(setDrawingColor(QColor)));
    connect(contextMenu, SIGNAL(penWidthIncrease()), drawScreenshot, SLOT(increasePenWidth()));
    connect(contextMenu, SIGNAL(penWidthDecrease()), drawScreenshot, SLOT(decreasePenWidth()));

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
    QString selectedContent;
    ContentSelector csel;
    if(csel.exec() != QDialog::Accepted)
        // no content selected, do nothing
        return;

    widgetStack->setVisible(true);

    // TODO implement context saving when we switch from one content type to the other!
    // e.g presentation -> another presentation saves the annotations, but presentation -> video
    // just hides the presentation and doesn't save the changes that was made on the presentation
    // it is still saved when we close the app but we shouldn't rely on that

    selectedContent = csel.getSelectedContent();

    if(selectedContent == "$screenshot$") {
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
            draw->raise();

        } else if(selectedContent.endsWith("mp4") || selectedContent.endsWith("avi") || selectedContent.endsWith("flv")) {

            widgetStack->setCurrentIndex(VIDEO_ANNOTATION);

            widgetStack->resize(ui->scrollArea->size()-QSize(10,10));
            videoDraw->attachToContentDisplay(videoPlayer);
            videoPlayer->selectContent(selectedContent);
            videoPlayer->raise();
            videoDraw->raise();

        } else if(selectedContent.endsWith("html") || selectedContent.endsWith("htm") || selectedContent.startsWith("www") || selectedContent.startsWith("http")) {
            // TODO webpage resize problems
            // TODO find a way to scroll the webpage and interact with links - enable disable annotation?
            widgetStack->setCurrentIndex(WEBPAGE_ANNOTATION);
            widgetStack->resize(ui->scrollArea->size()-QSize(10,10));
            groupBoxForWeb->resize(ui->scrollArea->size()-QSize(10,10));
            webDraw->attachToContentDisplay(webDisplay);
            webDisplay->selectContent(selectedContent);
            webDisplay->raise();
            webDraw->raise();
        }
    }
}

void MainWindow::openExistingSketch()
{
    QSize newSize = ui->scrollArea->size()-QSize(10,10);
    QImage blankImage(newSize,QImage::Format_MonoLSB);
    blankImage.fill(255);
    blankImage = blankImage.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    screenshotOrBlankImage->setPixmap(QPixmap::fromImage(blankImage));

    widgetStack->setCurrentIndex(SKETCHING_WIDGET);
    widgetStack->resize(blankImage.size());

    drawSketch->setVisible(true);
    drawScreenshot->setVisible(false);
    drawSketch->raise();
}

void MainWindow::openNewSketch()
{
    QSize newSize = ui->scrollArea->size()-QSize(10,10);
    QImage blankImage(newSize,QImage::Format_MonoLSB);
    blankImage.fill(255);
    blankImage = blankImage.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    screenshotOrBlankImage->setPixmap(QPixmap::fromImage(blankImage));

    drawSketch->getDrawingData()->clear();

    widgetStack->setCurrentIndex(SKETCHING_WIDGET);
    widgetStack->resize(blankImage.size());

    drawSketch->setVisible(true);
    drawScreenshot->setVisible(false);
    drawSketch->raise();
}

void MainWindow::openScreenshot()
{
    hide();
    scrnsht->show();
}

void MainWindow::getScreenshot()
{
    scrnsht->hide();
    usleep(5000);   // time for scrnsht window to be hided.

    QPixmap originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    showFullScreen();

    QImage screenshot = originalPixmap.toImage();
    QSize newSize = ui->scrollArea->size()-QSize(10,10);
    screenshot = screenshot.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    screenshotOrBlankImage->setPixmap(QPixmap::fromImage(screenshot));

    drawScreenshot->getDrawingData()->clear();

    widgetStack->setCurrentIndex(SKETCHING_WIDGET);
    widgetStack->resize(screenshot.size());

    drawSketch->setVisible(false);
    drawScreenshot->setVisible(true);
    drawScreenshot->raise();
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
    videoDraw->contentChanged("");
    webDraw->contentChanged("");
    recentlyUsed->writeToStorage();
    // TODO BUG recent items list is lost on closing the app?
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
