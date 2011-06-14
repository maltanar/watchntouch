/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include <QResizeEvent>
#include <QFileDialog>
#include "contentdisplaytask.h"
#include "eventgenerator.h"

ContentDisplayTask::ContentDisplayTask(QWidget *parent) :
    QWidget(parent)
{
    m_annotationWidget = NULL;
    m_contextMenu = NULL;
    m_contentDisplay = NULL;
    m_panel = NULL;
}

int ContentDisplayTask::getTaskType()
{
    return -1;
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
        connect(m_contextMenu, SIGNAL(eraserSize(int)), m_annotationWidget, SLOT(setEraserSize(int)));
        connect(m_contextMenu, SIGNAL(erasePage()), m_annotationWidget, SLOT(clear()));
        connect(m_contextMenu, SIGNAL(eraseAll()), m_annotationWidget, SLOT(clearAll()));
        connect(m_contextMenu, SIGNAL(print()), this, SLOT(print()));
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

    // connect to pinch gesture source
    connect(eventGenerator, SIGNAL(pinchGesture(QPoint,bool)), this, SLOT(pinchGesture(QPoint,bool)));

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

    // disconnect from pinch gesture source
    disconnect(eventGenerator, SIGNAL(pinchGesture(QPoint,bool)), this, SLOT(pinchGesture(QPoint,bool)));

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

QImage ContentDisplayTask::getTaskScreenshot(int maxHeight)
{
    QImage resultImage;
    QPixmap resultPixmap;

    resultPixmap = QPixmap::grabWidget(this);

    if(resultPixmap.height() > maxHeight && maxHeight != -1)
        resultImage = resultPixmap.toImage().scaledToHeight(maxHeight, Qt::SmoothTransformation);
    else
        resultImage = resultPixmap.toImage();

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

void ContentDisplayTask::pinchGesture(QPoint center, bool inOut)
{
    qWarning() << this << "received pinchGesture at" << center << "type" << inOut;
    showContextMenu(center);
}

void ContentDisplayTask::print()
{
    m_contextMenu->hideContextMenu();

    QImage thePrint = getTaskScreenshot(-1);
    QString fileName = QFileDialog::getSaveFileName(this, "Save annotated content", "",  "PNG files (*.png)");

    if(fileName != "") {
        if(!fileName.endsWith(".png")) fileName += ".png";
        thePrint.save(fileName, "PNG");
        displayInfoMessage("Annotated content saved to:\n"+fileName);
    }
}
