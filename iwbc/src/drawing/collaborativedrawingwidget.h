/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef COLLABORATIVEDRAWINGWIDGET_H
#define COLLABORATIVEDRAWINGWIDGET_H

#include "basedrawingwidget.h"
#include <QtNetwork>

class CollaborativeDrawingWidget : public BaseDrawingWidget
{
    Q_OBJECT
public:
    explicit CollaborativeDrawingWidget(QWidget *parent = 0);

protected:
    void commitDrawing(QPicture drawingData);
    void resizeEvent(QResizeEvent *event);

    QString m_currentSession;

signals:
    void drawingCommited(QString sessionName, QPicture drawingData);

public slots:
    void drawingArrived(QString sessionName, QByteArray picData, bool isInitialState);

};

#endif // COLLABORATIVEDRAWINGWIDGET_H
