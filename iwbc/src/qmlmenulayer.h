#ifndef QMLMENULAYER_H
#define QMLMENULAYER_H

#include <QDeclarativeView>

class QMLMenuLayer : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit QMLMenuLayer(QWidget *parent = 0);

protected:
    unsigned int m_maskHeight;

    void resizeEvent(QResizeEvent *event);



signals:

public slots:
    void adjustInteractiveHeight(int heightAdjustment);

};

#endif // QMLMENULAYER_H
