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

    setSource(QUrl("qrc:/qml/wtui.qml"));
    setAlignment(Qt::AlignBottom);
    setStyleSheet("background: transparent");
    // TODO MAJOR adjust mask rect size according to menu's current size!

}

void QMLMenuLayer::resizeEvent(QResizeEvent *event)
{
    QDeclarativeView::resizeEvent(event);

    clearMask();
    setMask(QRegion(0, height()-100, width() , 100));
}
