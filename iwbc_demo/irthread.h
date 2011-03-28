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

// TODO move these to appglobals
#define MOUSE_PRESSED 0
#define MOUSE_MOVE 1
#define MOUSE_RELEASED 2

class IRThread : public QThread
{

    Q_OBJECT

public:
    IRThread();
    virtual ~IRThread();

public:
    void run(); // From QThread

signals:
    void IRInputReceived(int x,int y,int i,int type,int count);
    void connected();

private:
    wiimote** wiimotes;
    int found;
    int isConnected;
    QPoint currentPoint;
    QPoint previousPoint;
    bool previous[4];

    void initialize();


};

#endif // IRTHREAD_H
