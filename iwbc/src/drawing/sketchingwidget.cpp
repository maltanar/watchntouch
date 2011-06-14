/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "sketchingwidget.h"

#include <QFile>
#include <QResizeEvent>

SketchingWidget::SketchingWidget(QWidget *parent) :
    BaseDrawingWidget(parent)
{

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setStyleSheet("background: white");

    currentContent = "sketch";
}


void SketchingWidget::resizeEvent(QResizeEvent *event)
{
    BaseDrawingWidget::resizeEvent(event);

    qWarning() << "resizing scene to" << size();

    getDrawingData()->setSceneRect(0,0,width(), height());
    getDrawingData()->requestStageSize(size());
}


void SketchingWidget::contentChanged(QString newContent)
{
    // content won't ever change for sketches
}

void SketchingWidget::contextChanged(QString newContext)
{
    if(currentContent != "" && getDrawingData()->isModified())
        getDrawingData()->saveImage(getCurrentAnnotation());

    currentContext = newContext;

    // attempt to load existing annotation for the new context
    if(QFile::exists(getCurrentAnnotation())) {
        getDrawingData()->loadImage(getCurrentAnnotation());
    } else {
        getDrawingData()->clear();
    }
}

QString SketchingWidget::getCurrentAnnotation()
{
    return matcher.matchingAnnotation(currentContent, currentContext);
}
