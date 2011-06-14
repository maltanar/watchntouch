/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

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
#define STATE_FINISH_GESTURING  6
#define STATE_SINGLE_IGNORE     7
#define STATE_FV_RELEASE        8 // galiba draw_release ile aynı amaçla yazılmış.

#define VERTICAL_SWIPE_SENSITIVITY   30
#define PINCH_THRESHOLD         50
#define GESTURE_START_DELAY     300

class EventGenerator : public QObject
{
    Q_OBJECT
public:
    explicit EventGenerator(QObject *parent = 0);
    ~EventGenerator();

    void processInputData(QPoint* pt,int i,int type,int visibleCount);
    void recognizeGesture(QPoint* newpoints);
    void finishGesture();

private:
    int m_currentState;
    QList<QPoint> m_pointBuffer;
    QList<QPoint> m_firstPointDifferenceBuffer;
    QList<QPoint> m_secondPointDifferenceBuffer;
    QPoint m_firstPreviousPoint;
    QPoint m_secondPreviousPoint;
    QPoint m_gestureStartPoint;
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
    void pinchGesture(QPoint center, bool inOut);
    void swipeGesture(int direction, int dx, int dy);

protected slots:
    void timeout();

public slots:

};

#endif // EVENTGENERATOR_H
