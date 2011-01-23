#ifndef BASEDRAWINGWIDGET_H
#define BASEDRAWINGWIDGET_H

#include <QGraphicsView>
#include <QResizeEvent>
#include "drawingdata.h"

// define the default tools for drawing
#define DEFAULT_DRAWING_PEN     QPen(Qt::black)
#define DEFAULT_DRAWING_BRUSH   QBrush(Qt::transparent, Qt::SolidPattern)
#define DEFAULT_ERASER          QPen(Qt::red)

typedef enum _DrawingMode {
    DRAWINGMODE_FREEHAND,
    DRAWINGMODE_ERASER,
    DRAWINGMODE_STRAIGHTLINE,
    DRAWINGMODE_ARROW,
    DRAWINGMODE_RECTANGLE,
    DRAWINGMODE_ELLIPSE
} DrawingMode;

typedef enum _DrawingState {
    DRAWINGSTATE_START,
    DRAWINGSTATE_UPDATE,
    DRAWINGSTATE_END
} DrawingState;

class BaseDrawingWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BaseDrawingWidget(QWidget *parent = 0);
    ~BaseDrawingWidget();

    void setDrawingData(DrawingData * newData);
    DrawingData * getDrawingData();    
    QPen getDrawingPen();
    QBrush getDrawingBrush();
    DrawingMode getDrawingMode();

protected:
    void mouseMoveEvent (QMouseEvent * event);
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
    void resizeEvent (QResizeEvent * event);

private:
    QPointF mouseDownPoint;
    QPointF mouseUpPoint;
    QPointF mousePrevPoint;
    DrawingData * drawingData;
    QPen drawingPen;
    QBrush drawingBrush;
    QPen eraserPen;
    DrawingMode drawingMode;
    QGraphicsItem * currentItem;


    void drawLineAction(QPointF from, QPointF to);
    void drawingStart(QPointF startPoint);
    void drawingUpdate(QPointF updatePoint);
    void drawingEnd(QPointF endPoint);
    void handleDrawingState(DrawingState state, QPointF lastPoint);

signals:

public slots:
    void setDrawingBrush(QBrush brush);
    void setDrawingPen(QPen pen);
    void setDrawingMode(DrawingMode mode);

};

#endif // BASEDRAWINGWIDGET_H
