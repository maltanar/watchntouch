#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include <QObject>
#include <QPoint>
#include "QjtMouseGestureFilter.h"
#include "QjtMouseGesture.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

class EventGenerator : public QObject
{
    Q_OBJECT
public:
    explicit EventGenerator(QObject *parent = 0);
    ~EventGenerator();

    void processInputData(QPoint pt,int i,int type,int visibleCount);

    void addGesture(QjtMouseGesture * gesture);
    void clearGestures();

private:
    // X display variable to generate pointer events
    Display * dpy;
    // vars related to smoothing
    QPointF smoothPoints[10];
    int smoothCounter;
    bool full;
    // state variables
    QPoint prev_point;
    int prev_i;
    int prev_type;
    int prev_visibleCount;
    bool isMouseButtonDown;
    // filter for gesture recognizer
    QjtMouseGestureFilter *gestureFilter;

    void mousePress(int button,QPoint p);
    void mouseMove(int button,QPoint p);
    void mouseRelease(int button,QPoint p);

    QPoint applySmoothing(QPoint inputPoint);

    void updateState(QPoint pt,int i,int type,int visibleCount);

signals:

public slots:

};

#endif // EVENTGENERATOR_H
