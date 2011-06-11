#include "contentdisplaytask.h"
#include <QResizeEvent>

ContentDisplayTask::ContentDisplayTask(QWidget *parent) :
    QWidget(parent)
{
    m_annotationWidget = NULL;
    m_contextMenu = NULL;
    m_contentDisplay = NULL;
    m_panel = NULL;
}

void ContentDisplayTask::setPanel(QObject *panel)
{
    m_panel = panel;
}

void ContentDisplayTask::setContextMenu(ContextMenu *newMenu)
{
    m_contextMenu = newMenu;
    m_contextMenu->hide();

    if(m_annotationWidget) {
        connect(m_annotationWidget, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

        // connect the context menu signals/slots with the annotation widget
        connect(m_contextMenu,SIGNAL(colorSelected(QColor)),m_annotationWidget, SLOT(setDrawingColor(QColor)));
        connect(m_contextMenu, SIGNAL(undo()), m_annotationWidget->getDrawingData()->getUndoStack(), SLOT(undo()));
        connect(m_contextMenu, SIGNAL(redo()), m_annotationWidget->getDrawingData()->getUndoStack(), SLOT(redo()));
        connect(m_contextMenu, SIGNAL(toolSelected(DrawingMode)), m_annotationWidget, SLOT(setDrawingMode(DrawingMode)));
        connect(m_contextMenu, SIGNAL(penWidthIncrease()), m_annotationWidget, SLOT(increasePenWidth()));
        connect(m_contextMenu, SIGNAL(penWidthDecrease()), m_annotationWidget, SLOT(decreasePenWidth()));
    }
}

int ContentDisplayTask::getPanelHeight()
{
    return 0;
}

void ContentDisplayTask::activate()
{
    // make this task the active task
    // at the base level this means the associated ContentDisplay will grab the keyboard
    // and the menu opening gesture (pinch)

    if(m_contentDisplay) {
        // TODO do we really need to grab the keyboard? find a better way
        // m_contentDisplay->grabKeyboard();
        // TODO IMPORTANT GESTURE grab pinch gesture!
    }

    showHidePanel(true);
}

void ContentDisplayTask::deactivate()
{
    // we are no longer the active task
    // for the base implementation, release the grabbed keyboard and gesture focus

    if(m_contentDisplay) {
        // m_contentDisplay->releaseKeyboard();
        // TODO IMPORTANT GESTURE release the pinch gesture
    }

    if(m_annotationWidget) {
        // force save the current content
        m_annotationWidget->requestSave();
    }

    showHidePanel(false);
}

void ContentDisplayTask::setAnnotationWidget(AnnotationWidget * newWidget)
{
    m_annotationWidget = newWidget;

    if(m_contentDisplay)
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
    // context menu coordinates are global
    p = mapFromGlobal(p);
    qWarning() << "caylar da caymis hani...o zaman context menu acalim. hoppala yavrum kokakola.";
    //m_contextMenu->setParent(qApp->topLevelWidgets().at(0));
    m_contextMenu->move(p - QPoint(m_contextMenu->width()/2, m_contextMenu->height()/2));
    m_contextMenu->show();
    m_contextMenu->raise();
}

void ContentDisplayTask::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    QSize hede = size();

    if(m_contentDisplay) {
        m_contentDisplay->setDesiredSize(hede);
        qWarning() << "ContentDisplayTask resizeEvent" << hede;
    }
}

QImage ContentDisplayTask::getTaskScreenshot()
{
    QImage resultImage;

    resultImage = QPixmap::grabWidget(this).toImage().scaledToHeight(120, Qt::SmoothTransformation);

    return resultImage;
}


// Route all mouse events received from QML to the annotation widget
void ContentDisplayTask::mousePress(QPoint p, int button, int buttons)
{
    // qWarning() << "mousePress" << p << button << buttons;

    QWidget * theChild = childAt(mapFromGlobal(p));
    qWarning() << "child" << theChild;
    if(theChild)
        qApp->postEvent(theChild, new QMouseEvent(QEvent::MouseButtonPress, theChild->mapFromGlobal(p), p, (Qt::MouseButton) button, (Qt::MouseButtons) buttons, 0));
}

void ContentDisplayTask::mouseMove(QPoint p, int button, int buttons)
{
    // qWarning() << "mouseMove" << p << button << buttons;
    QWidget * theChild = childAt(mapFromGlobal(p));
    if(theChild)
        qApp->postEvent(theChild, new QMouseEvent(QEvent::MouseMove, theChild->mapFromGlobal(p), p, (Qt::MouseButton) button, (Qt::MouseButtons) buttons, 0));
}

void ContentDisplayTask::mouseRelease(QPoint p, int button, int buttons)
{
    // qWarning() << "mouseRelease" << p << button << buttons;
    QWidget * theChild = childAt(mapFromGlobal(p));
    if(theChild)
        qApp->postEvent(theChild, new QMouseEvent(QEvent::MouseButtonRelease, theChild->mapFromGlobal(p), p, (Qt::MouseButton) button, (Qt::MouseButtons) buttons, 0));
}

