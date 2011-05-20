#ifndef QMLMENULAYER_H
#define QMLMENULAYER_H

#include <QDeclarativeView>

class QMLMenuLayer : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit QMLMenuLayer(QWidget *parent = 0);

    void setDeliveryWidget(QWidget *newWidget);
    QWidget * getDeliveryWidget();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    QWidget * m_deliveryWidget;
    bool m_deliveryActive;

signals:

public slots:

};

#endif // QMLMENULAYER_H
