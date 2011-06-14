/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef COLLABORATIVEDRAWINGTASK_H
#define COLLABORATIVEDRAWINGTASK_H

#include "contentdisplaytask.h"
#include "collaborativedrawingwidget.h"
#include "contextmenu.h"
#include "collaborationclient.h"
#include "collaborationserver.h"
#include <QVBoxLayout>
#include <QHash>

#define COLLABORATION_USERNAME  "Teacher"

class CollaborativeDrawingTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit CollaborativeDrawingTask(QWidget *parent = 0);

    int getTaskType();

protected:
    CollaborationClient *client;
    CollaborationServer *server;
    ProtocolHandler *ph;
    MessageTransceiver *mt;
    CollaborativeDrawingWidget * m_collaborationWidget;
    QVBoxLayout m_layout;

    void showHidePanel(bool show);

signals:

protected slots:
    void drawingCommitted(QString sessionName, QPicture pictureData);
    void sessionJoinResult(QString sessionName, QChar result, QHash<QString, long> users);
    void loginResult(bool result,QString msg);

public slots:

};

#endif // COLLABORATIVEDRAWINGTASK_H
