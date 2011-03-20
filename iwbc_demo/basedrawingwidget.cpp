#include "basedrawingwidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsLineItem>

#include "appglobals.h"


BaseDrawingWidget::BaseDrawingWidget(QWidget *parent) :
    QGraphicsView(parent)
{
    // allocate DrawingData
    setDrawingData(new DrawingData(this));
    // set the default options for drawing
    drawingPen = DEFAULT_DRAWING_PEN;
    drawingBrush = DEFAULT_DRAWING_BRUSH;
    drawingMode = DRAWINGMODE_FREEHAND;
    // set background as transparent
    setStyleSheet("background: transparent");

}

BaseDrawingWidget::~BaseDrawingWidget()
{

}

void BaseDrawingWidget::mouseMoveEvent (QMouseEvent * event)
{
    if(event->buttons() & Qt::LeftButton)
        drawingUpdate(mapToScene(event->pos()));
}

void BaseDrawingWidget::mousePressEvent (QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
        drawingStart(mapToScene(event->pos()));
}

void BaseDrawingWidget::mouseReleaseEvent (QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
        drawingEnd(mapToScene(event->pos()));
}

void BaseDrawingWidget::setDrawingData(DrawingData * newData)
{
    // TODO what to do with the old data?
    drawingData = newData;
    setScene(drawingData);
}

DrawingData * BaseDrawingWidget::getDrawingData()
{
    return drawingData;
}

void BaseDrawingWidget::setDrawingPen(QPen pen)
{
    drawingPen = pen;
}

QPen BaseDrawingWidget::getDrawingPen()
{
    return drawingPen;
}

void BaseDrawingWidget::setDrawingBrush(QBrush brush)
{
    drawingBrush = brush;
}

QBrush BaseDrawingWidget::getDrawingBrush()
{
    return drawingBrush;
}

void BaseDrawingWidget::setDrawingMode(DrawingMode mode)
{
    drawingMode = mode;
}

DrawingMode BaseDrawingWidget::getDrawingMode()
{
    return drawingMode;
}

void BaseDrawingWidget::drawingStart(QPointF startPoint)
{
    handleDrawingState(DRAWINGSTATE_START, startPoint);
    mouseDownPoint = mousePrevPoint = startPoint;
}

void BaseDrawingWidget::drawingUpdate(QPointF updatePoint)
{
    handleDrawingState(DRAWINGSTATE_UPDATE, updatePoint);
    mousePrevPoint = updatePoint;
}

void BaseDrawingWidget::drawingEnd(QPointF endPoint)
{
    handleDrawingState(DRAWINGSTATE_END, endPoint);
    //getDrawingData()->registerAction();
    mousePrevPoint = mouseUpPoint = endPoint;
}

void BaseDrawingWidget::resizeEvent(QResizeEvent * event)
{
    if(drawingData != NULL) {
        drawingData->setSceneRect(0,0,width(),height());
        qWarning() << "view:" << geometry() << "scene: " << drawingData->sceneRect();
    }

    QGraphicsView::resizeEvent(event);
}

void BaseDrawingWidget::handleDrawingState(DrawingState state, QPointF lastPoint)
{
    // handle drawing start/update/end events for the current drawing mode

    QPainter::CompositionMode prevCompMode;

    if(state == DRAWINGSTATE_START) {
        // start a new series of paint operations on the picture
        picturePainter.begin(&picture);
        // set the drawing style for the painter
        picturePainter.setPen(drawingPen);
        picturePainter.setBrush(drawingBrush);
    }

    switch(drawingMode) {
        case DRAWINGMODE_FREEHAND:
            if(state == DRAWINGSTATE_START) {
                QGraphicsLineItem * newLine = new QGraphicsLineItem(QLineF(lastPoint, lastPoint), 0, getDrawingData());
                currentItem = newLine;
                newLine->setPen(drawingPen);
            }

            else if(state == DRAWINGSTATE_UPDATE) {
                QGraphicsLineItem * newLine = new QGraphicsLineItem(currentItem, getDrawingData());
                newLine->setLine(QLineF(mousePrevPoint, lastPoint));
                newLine->setPen(drawingPen);
                picturePainter.drawLine(mousePrevPoint, lastPoint);
            }

            else {
                QGraphicsLineItem * newLine = new QGraphicsLineItem(currentItem, getDrawingData());
                newLine->setLine(QLineF(mousePrevPoint, lastPoint));
                newLine->setPen(drawingPen);
                // remove the temporary QGraphicsItem
                getDrawingData()->removeItem(currentItem);
                picturePainter.drawLine(mousePrevPoint, lastPoint);
             }

            break;

        case DRAWINGMODE_RECTANGLE:
            if(state == DRAWINGSTATE_START) {
                // create a temporary QGraphicsItem
                // will be committed to the drawing when the mouse is released
                QGraphicsRectItem * newRect = new QGraphicsRectItem(lastPoint.x(), lastPoint.y(),0,0);
                currentItem = (QGraphicsItem *) newRect;
                newRect->setPen(drawingPen);
                newRect->setBrush(drawingBrush);
                drawingData->addItem(currentItem);
            }

            else if(state == DRAWINGSTATE_UPDATE)
                // update the temporary QGraphicsItem
                ((QGraphicsRectItem*)currentItem)->setRect(QRectF(mouseDownPoint,lastPoint).normalized());

            else {
                // remove the temporary QGraphicsItem
                getDrawingData()->removeItem(currentItem);
                // commit the drawing to the stage pixmap
                picturePainter.drawRect(QRectF(mouseDownPoint,lastPoint).normalized());
            }
            break;

        case DRAWINGMODE_STRAIGHTLINE:
            if(state == DRAWINGSTATE_START) {
                // create a temporary QGraphicsItem
                // will be committed to the drawing when the mouse is released
                QGraphicsLineItem * newLine = new QGraphicsLineItem(QLineF(lastPoint, lastPoint));
                currentItem = (QGraphicsItem*) newLine;
                newLine->setPen(drawingPen);
                getDrawingData()->addItem(newLine);
            }

            else if(state == DRAWINGSTATE_UPDATE) {
                // update the temporary QGraphicsItem
                ((QGraphicsLineItem*)currentItem)->setLine(QLineF(mouseDownPoint, lastPoint));
            }

            else {
                // remove the temporary QGraphicsItem
                getDrawingData()->removeItem(currentItem);
                // commit the drawing to the stage pixmap
                picturePainter.drawLine(QLineF(mouseDownPoint, lastPoint));
            }
            break;

        case DRAWINGMODE_ELLIPSE:
            if(state == DRAWINGSTATE_START) {
                // create a temporary QGraphicsItem
                // will be committed to the drawing when the mouse is released
                QGraphicsEllipseItem * newRect = new QGraphicsEllipseItem(lastPoint.x(), lastPoint.y(),0,0);
                currentItem = (QGraphicsItem *) newRect;
                newRect->setPen(drawingPen);
                newRect->setBrush(drawingBrush);
                drawingData->addItem(currentItem);
            }

            else if(state == DRAWINGSTATE_UPDATE)
                // update the temporary QGraphicsItem
                ((QGraphicsRectItem*)currentItem)->setRect(QRectF(mouseDownPoint,lastPoint).normalized());

            else {
                // remove the temporary QGraphicsItem
                getDrawingData()->removeItem(currentItem);
                // commit the drawing to the stage pixmap
                picturePainter.drawEllipse(QRectF(mouseDownPoint,lastPoint).normalized());
            }
            break;

        case DRAWINGMODE_ERASER:
            // we have to set a specific composition mode for the eraser
            // back up the current value
            prevCompMode = picturePainter.compositionMode();
            picturePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            // fill the region to be erased with transparent color
            picturePainter.fillRect(QRectF(lastPoint, QSizeF(drawingPen.width()+5, drawingPen.width()+5)),Qt::transparent);
            // restore the old composition mode
            picturePainter.setCompositionMode(prevCompMode);
            break;

        case DRAWINGMODE_ARROW:
            // TODO not yet implemented - implement this as well
        break;

    }

    if(state == DRAWINGSTATE_END) {
        // finalize the painting on the QPicture
        picturePainter.end();
        // TODO do this with some kind of registration function
        getDrawingData()->registerAction(picture);
        getDrawingData()->setModified(true);
        // update the stage
        getDrawingData()->update(picture.boundingRect().adjusted(-drawingPen.width()-5,
                                                             -drawingPen.width()-5,
                                                             drawingPen.width()+5,
                                                             drawingPen.width()+5));
    }
}

void BaseDrawingWidget::setDrawingColor(QColor color)
{
    drawingPen.setColor(color);
}

void BaseDrawingWidget::increasePenWidth()
{
    drawingPen.setWidth(drawingPen.width()+1);
}

void BaseDrawingWidget::decreasePenWidth()
{
    drawingPen.setWidth(drawingPen.width() > 2 ? drawingPen.width()-1 : 1);
}
