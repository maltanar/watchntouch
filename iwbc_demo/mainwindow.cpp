#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStackedLayout>
#include <QGroupBox>
#include "contentselector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    display = new PresentationDisplayWidget(this);
    draw = new AnnotationWidget(this);

    QGroupBox *groupBox = new QGroupBox(this);

    groupBox->setFlat(true);

    QStackedLayout *layout = new QStackedLayout();

    layout->addWidget(display);
    layout->addWidget(draw);
    layout->setStackingMode(QStackedLayout::StackAll);
    groupBox->setLayout(layout);

    ui->scrollArea->setWidget(groupBox);

    connect(ui->actionNext, SIGNAL(triggered()), display, SLOT(gotoNextSlide()));
    connect(ui->actionPrevious, SIGNAL(triggered()), display, SLOT(gotoPrevSlide()));

    connect(ui->actionUndo, SIGNAL(triggered()), draw->getDrawingData()->getUndoStack(),SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered()), draw->getDrawingData()->getUndoStack(),SLOT(redo()));

    draw->attachToContentDisplay(display);
    draw->setStyleSheet("background: transparent");

    openContent();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openContent()
{
    ContentSelector csel;
    if(csel.exec() != QDialog::Accepted)
        // no content selected, do nothing
        return;

    // TODO check content type before loading?
    display->selectContent(csel.getSelectedContent());
    draw->raise();
}

void MainWindow::on_actionOpen_triggered()
{
    openContent();
}
