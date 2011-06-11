#ifndef IRTHREAD_H
#define IRTHREAD_H

#include <QThread>
#include <QCoreApplication>
#include <QPoint>

#include <stdio.h>
#include <stdlib.h>

#include "wiiuse.h"


#define HEIGHT_FIX  20  // TODO set this according to platform
#define MAX_WIIMOTES    1

class IRThread : public QThread
{

    Q_OBJECT

public:
    IRThread();
    virtual ~IRThread();

public:
    void run(); // From QThread

signals:
    void IRInputReceived(QPoint * irpoints,int i,int type,int count);
    void connected();

private:
    wiimote** wiimotes;
    int found;
    int isConnected;
    QPoint currentPoint;
    QPoint * previousPoints;
    bool previous[4];
    QPoint * irpoints;

    void initialize();
    void copyPoints(QPoint * p1, QPoint * p2);


};

#endif // IRTHREAD_H
