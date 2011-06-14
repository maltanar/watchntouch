#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawingdata.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "collaborativedrawingwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DrawingData *drawingData = new DrawingData(this);
    ui->graphicsView->setDrawingData(drawingData);
    connect(ui->actionUndo,SIGNAL(triggered()),ui->graphicsView->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),ui->graphicsView->getDrawingData()->getUndoStack(), SLOT(redo()));

    mt = new MessageTransceiver();
    ph = new ProtocolHandler();
    ph->setMessageTransceiver(mt);
    client = new CollaborationClient();
    client->setProtocolHandler(ph);

    mt->start();

    connect(client, SIGNAL(sessionJoinResult(QString,QChar,QHash<QString,long>)), this, SLOT(sessionJoinResult(QString,QChar,QHash<QString,long>)));
    connect(ui->graphicsView, SIGNAL(drawingCommited(QString,QPicture)), this, SLOT(drawingCommitted(QString,QPicture)));
    connect(client, SIGNAL(drawingArrived(QString,QByteArray,bool)), ui->graphicsView, SLOT(drawingArrived(QString,QByteArray,bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRedPen_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setColor(Qt::red);
    ui->graphicsView->setDrawingPen(current);
    ui->graphicsView->setDrawingMode(DRAWINGMODE_FREEHAND);
}

void MainWindow::on_actionBlackPen_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setColor(Qt::black);
    ui->graphicsView->setDrawingPen(current);
    ui->graphicsView->setDrawingMode(DRAWINGMODE_FREEHAND);
}

void MainWindow::on_actionBluePen_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setColor(Qt::blue);
    ui->graphicsView->setDrawingPen(current);
    ui->graphicsView->setDrawingMode(DRAWINGMODE_FREEHAND);
}

void MainWindow::on_actionPenWidthInc_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setWidth(current.width()+1);
    ui->graphicsView->setDrawingPen(current);
}

void MainWindow::on_actionPenWidthDec_triggered()
{
    QPen current = ui->graphicsView->getDrawingPen();
    current.setWidth(current.width()-1);
    ui->graphicsView->setDrawingPen(current);
}

void MainWindow::on_actionConnect_triggered()
{
    SessionJoinDialog * dlg = new SessionJoinDialog(client, this);
    dlg->show();
}

void MainWindow::on_actionEraser_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_ERASER);
}

void MainWindow::sessionJoinResult(QString sessionName, QChar result, QHash<QString, long> users)
{
    qWarning() << "Session join result: " << sessionName << " : " << result << " : " << users.count();
}

void MainWindow::drawingCommitted(QString sessionName, QPicture pictureData)
{
    QByteArray picData(pictureData.data(), pictureData.size());
    client->sendDrawing(sessionName, picData);
}

void MainWindow::on_actionStraightLine_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_STRAIGHTLINE);
}

void MainWindow::on_actionRectangle_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_RECTANGLE);
}

void MainWindow::on_actionFreehand_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_FREEHAND);
}

void MainWindow::on_actionClear_triggered()
{
    ui->graphicsView->clear();
}

void MainWindow::on_actionLeave_triggered()
{
    client->leaveSession(client->getActiveSession());
}
