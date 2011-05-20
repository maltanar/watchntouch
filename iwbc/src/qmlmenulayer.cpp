#include "qmlmenulayer.h"

#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsObject>

QMLMenuLayer::QMLMenuLayer(QWidget *parent) :
    QDeclarativeView(parent)
{
    this->installEventFilter(this);

    setSource(QUrl("qrc:/qml/wtui.qml"));
    setAlignment(Qt::AlignBottom);
    setStyleSheet("background: transparent");
    // TODO MAJOR adjust mask rect size according to menu's current size
    //setMask(QRegion(0, 500, 1000 ,1000));

    m_deliveryWidget = NULL;
    m_deliveryActive = false;
}


void QMLMenuLayer::mousePressEvent(QMouseEvent *event)
{
    // TODO just reset the clip rects on mouse events, don't deal with mouse handling
    /*if(!itemAt(event->pos()) && m_deliveryWidget) {
        QPoint mappedPoint = m_deliveryWidget->mapFromGlobal(event->globalPos());
        QMouseEvent *newEvent = new QMouseEvent(event->type(), mappedPoint, event->button(), event->buttons(), event->modifiers());
        qWarning() << "delivering press event" << newEvent->pos() << newEvent->globalPos();
        qApp->postEvent(m_deliveryWidget, newEvent);
        m_deliveryActive = true;
    } else*/
        QDeclarativeView::mousePressEvent(event);
}

void QMLMenuLayer::mouseMoveEvent(QMouseEvent *event)
{
    if(m_deliveryActive) {
        QPoint mappedPoint = m_deliveryWidget->mapFromGlobal(event->globalPos());
        QMouseEvent *newEvent = new QMouseEvent(event->type(), mappedPoint, event->button(), event->buttons(), event->modifiers());
        qWarning() << "delivering move event" << newEvent->pos() << newEvent->globalPos();
        qApp->postEvent(m_deliveryWidget, newEvent);
    } else
        QDeclarativeView::mouseMoveEvent(event);
}

void QMLMenuLayer::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_deliveryActive) {
        QPoint mappedPoint = m_deliveryWidget->mapFromGlobal(event->globalPos());
        QMouseEvent *newEvent = new QMouseEvent(event->type(), mappedPoint, event->button(), event->buttons(), event->modifiers());
        qWarning() << "delivering release event" << newEvent->pos() << newEvent->globalPos();
        qApp->postEvent(m_deliveryWidget, newEvent);
        m_deliveryActive = false;
    } else
        QDeclarativeView::mouseReleaseEvent(event);
}


void QMLMenuLayer::setDeliveryWidget(QWidget *newWidget)
{
    m_deliveryWidget = newWidget;
}

QWidget * QMLMenuLayer::getDeliveryWidget()
{
    return m_deliveryWidget ;
}
