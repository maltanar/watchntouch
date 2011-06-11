#include "annotationwidget.h"

#include <QFile>

AnnotationWidget::AnnotationWidget(QWidget *parent) :
    BaseDrawingWidget(parent)
{
    currentContentDisplay = NULL;
    m_isReadOnly = false;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void AnnotationWidget::requestSave()
{
    if(currentContent != "" && getDrawingData()->isModified()) {
        getDrawingData()->saveImage(getCurrentAnnotation());
        qWarning()<< "force saving annotation" << currentContent << currentContext;
    }
}

void AnnotationWidget::contentChanged(QString newContent)
{
    if(currentContent != "" && getDrawingData()->isModified()) {
        getDrawingData()->saveImage(getCurrentAnnotation());
        qWarning()<< "content change save girdi" << currentContent << currentContext;
    }

    currentContent = newContent;    // set new content identifier

    contextChanged("");    // context is initially empty for new content
}

void AnnotationWidget::contextChanged(QString newContext)
{
    if(currentContent != "" && getDrawingData()->isModified()) {
        getDrawingData()->saveImage(getCurrentAnnotation());
        qWarning()<< "context change save girdi" << currentContent << currentContext;
    }

    currentContext = newContext;

    if(currentContentDisplay && newContext != "") {
        currentSize = currentContentDisplay->getContentSize();  // set the content size
        qWarning() << "AnnotationWidget resizing to content size" << currentSize;
        //resize(currentSize); // resize the annotation widget
        getDrawingData()->setSceneRect(0,0,currentSize.width(), currentSize.height());
        getDrawingData()->requestStageSize(currentSize);
        sceneCenter = QPointF(width() / 2, height() / 2);
    }

    if(currentContent == "") {
        // hide annotations if no context is present
        setVisible(false);
    } else {
        setVisible(true);
    }

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
    // TODO operations regarding content size?

    if(currentContentDisplay == display)
        return;

    if(currentContentDisplay) {
        // remove the connections to the old widget
        disconnect(currentContentDisplay,SIGNAL(contentChanged(QString)), this, SLOT(contentChanged(QString)));
        disconnect(currentContentDisplay,SIGNAL(contextChanged(QString)), this, SLOT(contextChanged(QString)));
        disconnect(currentContentDisplay,SIGNAL(scrollRequested(int,int)), this, SLOT(scrollRequest(int,int)));
    }

    currentContentDisplay = display;

    contentChanged(display->getContentIdentifier());
    contextChanged(display->getContentContext());

    // establish the new connections
    connect(currentContentDisplay,SIGNAL(contentChanged(QString)), this, SLOT(contentChanged(QString)));
    connect(currentContentDisplay,SIGNAL(contextChanged(QString)), this, SLOT(contextChanged(QString)));
    connect(currentContentDisplay,SIGNAL(scrollRequested(int,int)), this, SLOT(scrollRequest(int,int)));
}

void AnnotationWidget::requestReadOnlyStatus(bool readOnly)
{
    qWarning() << "read only status" << readOnly;
    m_isReadOnly = readOnly;
    setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
}

void AnnotationWidget::scrollRequest(int dx, int dy)
{
    // TODO do we also need to pass the rect to be scrolled?
    //scroll(dx,dy);

    qWarning() << "prev scene center" << sceneCenter;
    sceneCenter -= QPointF(dx,dy);
    qWarning() << "new scene center" << sceneCenter;

    centerOn(sceneCenter);

}

bool AnnotationWidget::isReadOnly()
{
    return m_isReadOnly;
}
