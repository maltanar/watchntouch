#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include <QObject>
#include <QPoint>
#include <QList>
#include <QTimer>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#define STATE_NOTHING           0
#define STATE_FIRST_VISIBLE     1
#define STATE_NORMAL_DRAW       2
#define STATE_DRAW_RELEASE      3
#define STATE_SECOND_VISIBLE    4
#define STATE_GESTURING         5
#define STATE_SINGLE_IGNORE     6
#define STATE_FV_RELEASE        7

#define GESTURE_START_DELAY     100

class EventGenerator : public QObject
{
    Q_OBJECT
public:
    explicit EventGenerator(QObject *parent = 0);
    ~EventGenerator();

    void processInputData(QPoint* pt,int i,int type,int visibleCount);

private:
    int m_currentState;
    QList<QPoint> m_pointBuffer;
    QTimer m_timer;
    int m_visibleCount;
    bool m_timeoutFlag;

    void handleStateChange();

    // X display variable to generate pointer events
    Display * dpy;
    // vars related to smoothing
    QPointF smoothPoints[10];
    int smoothCounter;
    bool full;
    bool isMouseButtonDown;

    void mousePress(int button,QPoint p);
    void mouseMove(int button,QPoint p);
    void mouseRelease(int button,QPoint p);

    QPoint applySmoothing(QPoint inputPoint);

signals:

protected slots:
    void timeout();

public slots:

};

#endif // EVENTGENERATOR_H
