#include "basedrawingwidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsLineItem>


BaseDrawingWidget::BaseDrawingWidget(QWidget *parent) :
    QGraphicsView(parent)
{
    // allocate DrawingData
    setDrawingData(new DrawingData(this));
    // set the default options for drawing
    drawingPen = DEFAULT_DRAWING_PEN;
    drawingBrush = DEFAULT_DRAWING_BRUSH;
    eraserPen = QPen(Qt::transparent);
    drawingMode = DRAWINGMODE_FREEHAND;
}

BaseDrawingWidget::~BaseDrawingWidget()
{

}

void BaseDrawingWidget::mouseMoveEvent (QMouseEvent * event)
{
    drawingUpdate(mapToScene(event->pos()));
}

void BaseDrawingWidget::mousePressEvent (QMouseEvent * event)
{
    drawingStart(mapToScene(event->pos()));
}

void BaseDrawingWidget::mouseReleaseEvent (QMouseEvent * event)
{
    drawingEnd(mapToScene(event->pos()));
}

void BaseDrawingWidget::drawLineAction(QPointF from, QPointF to)
{
    QGraphicsLineItem *newItem = new QGraphicsLineItem(QLineF(from, to));
    newItem->setPen(drawingPen);
    getDrawingData()->addItem(newItem);
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
    getDrawingData()->registerAction();
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

    switch(drawingMode) {
        case DRAWINGMODE_FREEHAND:
            if(state == DRAWINGSTATE_START)
                // TODO maybe put a dot?
                ;

            else if(state == DRAWINGSTATE_UPDATE) {
                QGraphicsLineItem *newItem = new QGraphicsLineItem(QLineF(mousePrevPoint, lastPoint));
                newItem->setPen(drawingPen);
                getDrawingData()->addItem(newItem);
            }

            else {
                QGraphicsLineItem *newItem = new QGraphicsLineItem(QLineF(mousePrevPoint, lastPoint));
                newItem->setPen(drawingPen);
                getDrawingData()->addItem(newItem);
             }


        break;

        case DRAWINGMODE_RECTANGLE:
        if(state == DRAWINGSTATE_START) {
                QGraphicsRectItem * newRect = new QGraphicsRectItem(lastPoint.x(), lastPoint.y(),0,0);
                currentItem = (QGraphicsItem *) newRect;
                newRect->setPen(drawingPen);
                newRect->setBrush(drawingBrush);
                drawingData->addItem(currentItem);
            }

            else if(state == DRAWINGSTATE_UPDATE)
                ((QGraphicsRectItem*)currentItem)->setRect(QRectF(mouseDownPoint,lastPoint).normalized());

            else
                ((QGraphicsRectItem*)currentItem)->setRect(QRectF(mouseDownPoint,lastPoint).normalized());
        break;

        case DRAWINGMODE_STRAIGHTLINE:
        if(state == DRAWINGSTATE_START) {
            QGraphicsLineItem * newLine = new QGraphicsLineItem(QLineF(lastPoint, lastPoint));
            currentItem = (QGraphicsItem*) newLine;
            newLine->setPen(drawingPen);
            getDrawingData()->addItem(newLine);
        }

        else if(state == DRAWINGSTATE_UPDATE) {
            ((QGraphicsLineItem*)currentItem)->setLine(QLineF(mouseDownPoint, lastPoint));
        }

        else {
            ((QGraphicsLineItem*)currentItem)->setLine(QLineF(mouseDownPoint, lastPoint));
         }
        break;

        case DRAWINGMODE_ELLIPSE:
            if(state == DRAWINGSTATE_START) {
                    QGraphicsEllipseItem * newEllipse = new QGraphicsEllipseItem(lastPoint.x(), lastPoint.y(),0,0);
                    currentItem = (QGraphicsItem *) newEllipse;
                    newEllipse->setPen(drawingPen);
                    newEllipse->setBrush(drawingBrush);
                    drawingData->addItem(currentItem);
                }

                else if(state == DRAWINGSTATE_UPDATE)
                    ((QGraphicsEllipseItem*)currentItem)->setRect(QRectF(mouseDownPoint,lastPoint).normalized());

                else
                    ((QGraphicsEllipseItem*)currentItem)->setRect(QRectF(mouseDownPoint,lastPoint).normalized());
        break;

        case DRAWINGMODE_ERASER:
        case DRAWINGMODE_ARROW:
            // TODO not yet implemented - implement these as well
        break;
    }
}
