#include "sketchingwidget.h"

#include <QFile>

SketchingWidget::SketchingWidget(QWidget *parent) :
    BaseDrawingWidget(parent)
{

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


void SketchingWidget::contentChanged(QString newContent)
{
    if(currentContent != "" && getDrawingData()->isModified())
        getDrawingData()->saveImage(getCurrentAnnotation());

    currentContent = newContent;    // set new content identifier
    currentContext = "";    // context is initially empty for new content
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
