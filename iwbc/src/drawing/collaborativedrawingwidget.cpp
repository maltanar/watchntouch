/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "collaborativedrawingwidget.h"
#include <QResizeEvent>

CollaborativeDrawingWidget::CollaborativeDrawingWidget(QWidget *parent) :
    BaseDrawingWidget(parent)
{
    // disable scrollbars for collaboration area
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // collaboration widget is disabled until we join a session
    setEnabled(false);
}

void CollaborativeDrawingWidget::commitDrawing(QPicture drawingPictureData)
{
    qWarning() << "commitDrawing size" << drawingPictureData.size();

    BaseDrawingWidget::commitDrawing(drawingPictureData);

    emit drawingCommited(m_currentSession, drawingPictureData);

}

void CollaborativeDrawingWidget::drawingArrived(QString sessionName, QByteArray picData, bool isInitialState)
{
    // this slot will be invoked when the user joins a new session and the server
    // sends the current drawing state for this session
    QPicture pic;
    qWarning() << "Session state data of size : " << picData.size() << "from session" << sessionName;
    if(isInitialState) {
        // set the session name
        m_currentSession = sessionName;
        // clear up any old data
        getDrawingData()->clear();
        // the widget should be enabled now
        setEnabled(true);
    }
    pic.setData(picData.constData(), picData.size());

    // commit the new data
    BaseDrawingWidget::commitDrawing(pic);
}

void CollaborativeDrawingWidget::resizeEvent(QResizeEvent *event)
{
    BaseDrawingWidget::resizeEvent(event);

    qWarning() << "resizing scene to" << size();

    getDrawingData()->setSceneRect(0,0,width(), height());
    getDrawingData()->requestStageSize(size());
}
