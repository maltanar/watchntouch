#ifndef QMLMENULAYER_H
#define QMLMENULAYER_H

#include <QDeclarativeView>

class QMLMenuLayer : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit QMLMenuLayer(QWidget *parent = 0);

    void setMaskHeightFromBottom(unsigned int height);

protected:
    unsigned int m_maskHeight;

    void resizeEvent(QResizeEvent *event);



signals:

public slots:

};

#endif // QMLMENULAYER_H
