#include "annotationwidget.h"

#include <QFile>

AnnotationWidget::AnnotationWidget(QWidget *parent) :
    BaseDrawingWidget(parent)
{
    currentContentDisplay = NULL;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void AnnotationWidget::contentChanged(QString newContent)
{
    if(currentContent != "" && getDrawingData()->isModified()) {
        getDrawingData()->saveImage(getCurrentAnnotation());
        qWarning()<< "content change save girdi" << currentContent << currentContext;
    }

    currentContent = newContent;    // set new content identifier
    currentSize = currentContentDisplay->getContentSize();  // set the content size
    currentContext = "";    // context is initially empty for new content
}

void AnnotationWidget::contextChanged(QString newContext)
{
    if(currentContent != "" && getDrawingData()->isModified()) {
        getDrawingData()->saveImage(getCurrentAnnotation());
        qWarning()<< "context change save girdi" << currentContent << currentContext;
    }

    currentContext = newContext;

    // attempt to load existing annotation for the new context
    if(QFile::exists(getCurrentAnnotation())) {
        getDrawingData()->loadImage(getCurrentAnnotation());
    } else {
        getDrawingData()->clear();
    }
}

QString AnnotationWidget::getCurrentAnnotation()
{
    return matcher.matchingAnnotation(currentContent, currentContext);
}

void AnnotationWidget::attachToContentDisplay(ContentDisplay *display)
{
    if(currentContentDisplay == display)
        return;

    if(currentContentDisplay) {
        // remove the connections to the old widget
        disconnect(currentContentDisplay,SIGNAL(contentChanged(QString)), this, SLOT(contentChanged(QString)));
        disconnect(currentContentDisplay,SIGNAL(contextChanged(QString)), this, SLOT(contextChanged(QString)));
    }

    currentContentDisplay = display;

    contentChanged(display->getContentIdentifier());
    contextChanged(display->getContentContext());

    // establish the new connections
    connect(currentContentDisplay,SIGNAL(contentChanged(QString)), this, SLOT(contentChanged(QString)));
    connect(currentContentDisplay,SIGNAL(contextChanged(QString)), this, SLOT(contextChanged(QString)));
}

void AnnotationWidget::requestReadOnlyStatus(bool readOnly)
{
    qWarning() << "read only status" << readOnly;
    if(readOnly != m_isReadOnly)
    {
        m_isReadOnly = readOnly;
        setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
    }
}
