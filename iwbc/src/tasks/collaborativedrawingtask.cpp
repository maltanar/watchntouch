#include "collaborativedrawingtask.h"

CollaborativeDrawingTask::CollaborativeDrawingTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
    m_contextMenu = new ContextMenu(this);
    m_collaborationWidget = new CollaborativeDrawingWidget(this);

    m_layout.addWidget(m_collaborationWidget);

    setLayout(&m_layout);

    m_contextMenu->hide();

    connect(m_collaborationWidget, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

    // connect the context menu signals/slots with the sketching widget
    connect(m_contextMenu,SIGNAL(colorSelected(QColor)),m_collaborationWidget, SLOT(setDrawingColor(QColor)));
    connect(m_contextMenu, SIGNAL(undo()), m_collaborationWidget->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(m_contextMenu, SIGNAL(redo()), m_collaborationWidget->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(m_contextMenu, SIGNAL(toolSelected(DrawingMode)), m_collaborationWidget, SLOT(setDrawingMode(DrawingMode)));
    connect(m_contextMenu, SIGNAL(penWidthIncrease()), m_collaborationWidget, SLOT(increasePenWidth()));
    connect(m_contextMenu, SIGNAL(penWidthDecrease()), m_collaborationWidget, SLOT(decreasePenWidth()));
    connect(m_contextMenu, SIGNAL(eraserSize(int)), m_collaborationWidget, SLOT(setEraserSize(int)));
    connect(m_contextMenu, SIGNAL(erasePage()), m_collaborationWidget, SLOT(clear()));

    mt = new MessageTransceiver();
    ph = new ProtocolHandler();
    ph->setMessageTransceiver(mt);
    server = new CollaborationServer();
    server->setProtocolHandler(ph);
    client = new CollaborationClient();
    client->setProtocolHandler(ph);

    /*connect(client, SIGNAL(sessionJoinResult(QString,QChar,QHash<QString,long>)), this, SLOT(sessionJoinResult(QString,QChar,QHash<QString,long>)));
    connect(ui->graphicsView, SIGNAL(drawingCommited(QString,QPicture)), this, SLOT(drawingCommitted(QString,QPicture)));
    connect(client, SIGNAL(drawingArrived(QString,QByteArray,bool)), ui->graphicsView, SLOT(drawingArrived(QString,QByteArray,bool)));
    connect(client, SIGNAL(sessionListAvailable(QStringList)), this, SLOT(gotSessionList(QStringList)));*/

    connect(client, SIGNAL(sessionJoinResult(QString,QChar,QHash<QString,long>)), this, SLOT(sessionJoinResult(QString,QChar,QHash<QString,long>)));
    connect(m_collaborationWidget, SIGNAL(drawingCommited(QString,QPicture)), this, SLOT(drawingCommitted(QString,QPicture)));
    connect(client, SIGNAL(drawingArrived(QString,QByteArray,bool)), m_collaborationWidget, SLOT(drawingArrived(QString,QByteArray,bool)));
    connect(client, SIGNAL(loginResult(bool,QString)), this, SLOT(loginResult(bool,QString)));

    server->setServerUserName(COLLABORATION_USERNAME);

    mt->start();

    client->loginToServer(QHostAddress("127.0.0.1"), COLLABORATION_USERNAME, COLLABORATION_USERNAME);
    client->refreshSessionList();

}

void CollaborativeDrawingTask::loginResult(bool result, QString msg)
{
    qWarning() << "loginResult" <<  result << msg;

    if(result) {
        client->joinSession("Test","1234");
    }
}

void CollaborativeDrawingTask::drawingCommitted(QString sessionName, QPicture pictureData)
{
    QByteArray picData(pictureData.data(), pictureData.size());
    client->sendDrawing(sessionName, picData);
}

int CollaborativeDrawingTask::getTaskType()
{
    return TASK_COLALBORATION;
}

void CollaborativeDrawingTask::showHidePanel(bool show)
{
    // do nothing
}

void CollaborativeDrawingTask::sessionJoinResult(QString sessionName, QChar result, QHash<QString, long> users)
{
    //QMessageBox m;
    if(result == 1) {
        //m.setText("Joined session " + sessionName + ", you may start drawing");
        //m.exec();
        //ui->stackedWidget->setCurrentIndex(0);
        qWarning() << "session join successful!";
        m_collaborationWidget->setEnabled(true);
    } else {
        //m.setText("Failed to join session " + sessionName + ", check the password");
       // m.exec();
        //ui->stackedWidget->setCurrentIndex(1);
        qWarning() << "session join failed!";
    }

    qWarning() << "Session join result: " << sessionName << " : " << result << " : " << users.count();
}
