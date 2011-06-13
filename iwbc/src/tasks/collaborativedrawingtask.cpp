#include "collaborativedrawingtask.h"


#define COLLABORATION_USERNAME  "CollaborationServer"

CollaborativeDrawingTask::CollaborativeDrawingTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
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


    server->setServerUserName(COLLABORATION_USERNAME);
    mt->start();

    client->loginToServer(QHostAddress("127.0.0.1"), COLLABORATION_USERNAME, COLLABORATION_USERNAME);
    client->refreshSessionList();

}

int CollaborativeDrawingTask::getTaskType()
{
    return TASK_COLALBORATION;
}

void CollaborativeDrawingTask::showHidePanel(bool show)
{
    // do nothing
}
