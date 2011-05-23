#include "contentdisplaytask.h"
#include <QResizeEvent>

ContentDisplayTask::ContentDisplayTask(QWidget *parent) :
    QWidget(parent)
{
    m_annotationWidget = NULL;
    m_contextMenu = NULL;
    m_contentDisplay = NULL;

    setContextMenuPolicy(Qt::CustomContextMenu);
}

void ContentDisplayTask::setContextMenu(ContextMenu *newMenu)
{
    // remove the connections with old context menu, if exists
    disconnect(this);

    m_contextMenu = newMenu;
    m_contextMenu->hide();

     connect(this, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

    // connect the context menu signals/slots with the annotation widget
    connect(m_contextMenu,SIGNAL(colorSelected(QColor)),m_annotationWidget, SLOT(setDrawingColor(QColor)));
    connect(m_contextMenu, SIGNAL(undo()), m_annotationWidget->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(m_contextMenu, SIGNAL(redo()), m_annotationWidget->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(m_contextMenu, SIGNAL(toolSelected(DrawingMode)), m_annotationWidget, SLOT(setDrawingMode(DrawingMode)));
    connect(m_contextMenu, SIGNAL(penWidthIncrease()), m_annotationWidget, SLOT(increasePenWidth()));
    connect(m_contextMenu, SIGNAL(penWidthDecrease()), m_annotationWidget, SLOT(decreasePenWidth()));
}

void ContentDisplayTask::setAnnotationWidget(AnnotationWidget * newWidget)
{
    m_annotationWidget = newWidget;

    if(m_annotationWidget)
        m_annotationWidget->attachToContentDisplay(m_contentDisplay);
}

void ContentDisplayTask::setContentDisplay(ContentDisplay * newDisplay)
{
    m_contentDisplay = newDisplay;

    if(m_annotationWidget)
        m_annotationWidget->attachToContentDisplay(m_contentDisplay);

}

ContentDisplay * ContentDisplayTask::getContentDisplay()
{
    return m_contentDisplay;
}

AnnotationWidget * ContentDisplayTask::getAnnotationWidget()
{
    return m_annotationWidget;
}

ContextMenu * ContentDisplayTask::getContextMenu()
{
    return m_contextMenu;
}

void ContentDisplayTask::showContextMenu(QPoint p)
{
    qWarning() << "caylar da caymis hani...o zaman context menu acalim. hoppala yavrum kokakola.";
    m_contextMenu->move(p - QPoint(m_contextMenu->width()/2, m_contextMenu->height()/2));
    m_contextMenu->show();
    m_contextMenu->raise();
}

void ContentDisplayTask::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    if(m_contentDisplay)
        m_contentDisplay->setDesiredSize(size());
}
