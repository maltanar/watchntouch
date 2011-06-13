#ifndef COLLABORATIVEDRAWINGTASK_H
#define COLLABORATIVEDRAWINGTASK_H

#include "contentdisplaytask.h"
#include "collaborativedrawingwidget.h"
#include "contextmenu.h"
#include "collaborationclient.h"
#include "collaborationserver.h"
#include <QVBoxLayout>

class CollaborativeDrawingTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit CollaborativeDrawingTask(QWidget *parent = 0);

    int getTaskType();

protected:
    void showHidePanel(bool show);

    CollaborationClient *client;
    CollaborationServer *server;
    ProtocolHandler *ph;
    MessageTransceiver *mt;

signals:

public slots:

};

#endif // COLLABORATIVEDRAWINGTASK_H
