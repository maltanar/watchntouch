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

}

void QMLMenuLayer::adjustInteractiveHeight(int heightAdjustment)
{
    qWarning() << "ih adjustment" << heightAdjustment;
    qWarning() << "old ih" << m_maskHeight;
    m_maskHeight += heightAdjustment;
    qWarning() << "new ih" << m_maskHeight;

    clearMask();
    setMask(QRegion(0, height() - m_maskHeight, width() , m_maskHeight));
}

void QMLMenuLayer::resizeEvent(QResizeEvent *event)
{
    QDeclarativeView::resizeEvent(event);

    if(m_maskHeight) {
        clearMask();
        setMask(QRegion(0, height() - m_maskHeight, width() , m_maskHeight));
    }
}
