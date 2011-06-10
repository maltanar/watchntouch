#include "qmlmenulayer.h"

#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsObject>
#include <QResizeEvent>

QMLMenuLayer::QMLMenuLayer(QWidget *parent) :
    QDeclarativeView(parent)
{

    setSource(QUrl("qrc:/qml/wtui.qml"));
    setAlignment(Qt::AlignBottom);
    setStyleSheet("background: transparent");

    QRectF qmlRect = rootObject()->boundingRect();

    m_qmlWidth = qmlRect.width();
    m_qmlHeight = qmlRect.height();

    setQMLMask();

    connect(rootObject(), SIGNAL(mousePressed(int,int,int,int)), this, SLOT(qmlMousePressed(int,int,int,int)));
    connect(rootObject(), SIGNAL(mouseMoved(int,int,int,int)), this, SLOT(qmlMouseMoved(int,int,int,int)));
    connect(rootObject(), SIGNAL(mouseReleased(int,int,int,int)), this, SLOT(qmlMouseReleased(int,int,int,int)));

}

void QMLMenuLayer::setQMLMask()
{
    QRegion newMask = QRegion((width() - m_qmlWidth)/2, height() - m_qmlHeight, m_qmlWidth, m_qmlHeight);

    clearMask();
    setMask(newMask);
}

void QMLMenuLayer::resizeEvent(QResizeEvent *event)
{
    QDeclarativeView::resizeEvent(event);

    setQMLMask();
}

void QMLMenuLayer::qmlMousePressed(int x, int y, int button, int buttons)
{
    QPoint viewPoint = mapToGlobal(mapFromScene(QPointF(x,y)));
    emit mousePressSignal(viewPoint, button, buttons);
}

void QMLMenuLayer::qmlMouseMoved(int x, int y, int button, int buttons)
{
    QPoint viewPoint = mapToGlobal(mapFromScene(QPointF(x,y)));
    emit mouseMoveSignal(viewPoint, button, buttons);
}

void QMLMenuLayer::qmlMouseReleased(int x, int y, int button, int buttons)
{
    QPoint viewPoint = mapToGlobal(mapFromScene(QPointF(x,y)));
    emit mouseReleaseSignal(viewPoint, button, buttons);
}
