/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef BASEDRAWINGWIDGET_H
#define BASEDRAWINGWIDGET_H
#include "appglobals.h"
#include <QGraphicsView>
#include <QResizeEvent>
#include <QPicture>
#include <QPainter>
#include "drawingdata.h"



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
    virtual void commitDrawing(QPicture drawingPictureData);


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
    int m_eraserSize;

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
    void setEraserSize(int size);
    void increasePenWidth();
    void decreasePenWidth();
    void clear();

};

#endif // BASEDRAWINGWIDGET_H
