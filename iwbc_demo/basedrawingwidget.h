#ifndef BASEDRAWINGWIDGET_H
#define BASEDRAWINGWIDGET_H

#include <QGraphicsView>
#include <QResizeEvent>
#include <QPicture>
#include <QPainter>
#include "drawingdata.h"
#include "appglobals.h"


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
    DrawingMode drawingMode;
    QGraphicsItem * currentItem;
    QPixmap * stage;

    QPicture picture;
    QPainter picturePainter;

    void drawingStart(QPointF startPoint);
    void drawingUpdate(QPointF updatePoint);
    void drawingEnd(QPointF endPoint);
    void handleDrawingState(DrawingState state, QPointF lastPoint);

signals:

public slots:
    void setDrawingBrush(QBrush brush);
    void setDrawingPen(QPen pen);
    void setDrawingMode(DrawingMode mode);
    void setDrawingColor(QColor color);
    void increasePenWidth();
    void decreasePenWidth();

};

#endif // BASEDRAWINGWIDGET_H
