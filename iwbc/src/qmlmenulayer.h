#ifndef QMLMENULAYER_H
#define QMLMENULAYER_H

#include <QDeclarativeView>

class QMLMenuLayer : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit QMLMenuLayer(QWidget *parent = 0);



protected:

    void resizeEvent(QResizeEvent *event);



signals:

public slots:

};

#endif // QMLMENULAYER_H
