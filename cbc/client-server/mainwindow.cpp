#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drawingdata.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DrawingData *drawingData = new DrawingData(this);
    ui->graphicsView->setDrawingData(drawingData);
    connect(ui->actionUndo,SIGNAL(triggered()),ui->graphicsView->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),ui->graphicsView->getDrawingData()->getUndoStack(), SLOT(redo()));

    ui->stackedWidget->setCurrentIndex(1);
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

void MainWindow::on_actionEraser_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_ERASER);
}

void MainWindow::sessionJoinResult(QString sessionName, QChar result, QHash<QString, long> users)
{
    QMessageBox m;
    if(result == 1) {
        m.setText("Joined session " + sessionName + ", you may start drawing");
        m.exec();
        ui->stackedWidget->setCurrentIndex(0);
    } else {
        m.setText("Failed to join session " + sessionName + ", check the password");
        m.exec();
        ui->stackedWidget->setCurrentIndex(1);
    }

    qWarning() << "Session join result: " << sessionName << " : " << result << " : " << users.count();
}

void MainWindow::drawingCommitted(QString sessionName, QPicture pictureData)
{
    QByteArray picData(pictureData.data(), pictureData.size());
    client->sendDrawing(sessionName, picData);
}

void MainWindow::on_actionDrawing_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionSessions_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_startServerButton_clicked()
{
    mt = new MessageTransceiver();
    ph = new ProtocolHandler();
    ph->setMessageTransceiver(mt);
    server = new CollaborationServer();
    server->setProtocolHandler(ph);
    client = new CollaborationClient();
    client->setProtocolHandler(ph);

    connect(client, SIGNAL(sessionJoinResult(QString,QChar,QHash<QString,long>)), this, SLOT(sessionJoinResult(QString,QChar,QHash<QString,long>)));
    connect(ui->graphicsView, SIGNAL(drawingCommited(QString,QPicture)), this, SLOT(drawingCommitted(QString,QPicture)));
    connect(client, SIGNAL(drawingArrived(QString,QByteArray,bool)), ui->graphicsView, SLOT(drawingArrived(QString,QByteArray,bool)));

    connect(client, SIGNAL(sessionListAvailable(QStringList)), this, SLOT(gotSessionList(QStringList)));


    ui->startServerButton->setEnabled(false);
    ui->statusLabel->setText("Running");
    server->setServerUserName(ui->userName->text());
    mt->start();

    client->loginToServer(QHostAddress("127.0.0.1"), ui->userName->text(), ui->userName->text());
    client->refreshSessionList();

    ui->sessionBox->setEnabled(true);
}

void MainWindow::gotSessionList(QStringList sessionList)
{
        ui->sessionList->clear();
        ui->sessionList->addItems(sessionList);
}

void MainWindow::on_refreshList_clicked()
{
    client->refreshSessionList();
}

void MainWindow::on_pushButton_clicked()
{
    client->joinSession(ui->sessionList->currentItem()->text(), ui->sessionPassword->text());
}

void MainWindow::on_actionClear_triggered()
{
    ui->graphicsView->clear();
}

void MainWindow::on_actionFreehand_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_FREEHAND);
}

void MainWindow::on_actionRectangle_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_RECTANGLE);
}

void MainWindow::on_actionStraightLine_triggered()
{
    ui->graphicsView->setDrawingMode(DRAWINGMODE_STRAIGHTLINE);
}

void MainWindow::on_btn_createSession_clicked()
{
    client->createSession("sessozz", "1234");
}

void MainWindow::on_btn_leaveSession_clicked()
{
    client->leaveSession(client->getActiveSession());
    ui->stackedWidget->setCurrentIndex(1);
}
