#include "qmlmenulayer.h"

#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsObject>
#include <QResizeEvent>

QMLMenuLayer::QMLMenuLayer(QWidget *parent) :
    QDeclarativeView(parent)
{
    this->installEventFilter(this);
    m_maskHeight = 0;
    setSource(QUrl("qrc:/qml/wtui.qml"));
    setAlignment(Qt::AlignBottom);
    setStyleSheet("background: transparent");
    // TODO MAJOR adjust mask rect size according to menu's current size!

    connect(rootObject(), SIGNAL(mousePressed(int,int,int,int)), this, SLOT(qmlMousePressed(int,int,int,int)));
    connect(rootObject(), SIGNAL(mouseMoved(int,int,int,int)), this, SLOT(qmlMouseMoved(int,int,int,int)));
    connect(rootObject(), SIGNAL(mouseReleased(int,int,int,int)), this, SLOT(qmlMouseReleased(int,int,int,int)));

}

void QMLMenuLayer::resizeEvent(QResizeEvent *event)
{
    QDeclarativeView::resizeEvent(event);

    if(m_maskHeight) {
        clearMask();
        setMask(QRegion(0, height() - m_maskHeight, width() , m_maskHeight));
    }
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
