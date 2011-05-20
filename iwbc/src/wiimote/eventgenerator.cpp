#include "appglobals.h"
#include "eventgenerator.h"

EventGenerator::EventGenerator(QObject *parent) :
    QObject(parent)
{
    dpy = XOpenDisplay(NULL);

    smoothCounter = 0;
    full = false;

    for(int i = 0 ; i < 10; i++) {
        smoothPoints[i] = QPoint(0,0);
    }

    prev_point = QPoint(0,0);
    prev_i = 0;
    prev_type = -1;
    prev_visibleCount = 0;

    isMouseButtonDown = false;

    gestureFilter = new QjtMouseGestureFilter(Qt::RightButton,this);
}

EventGenerator::~EventGenerator()
{
    XCloseDisplay(dpy);
    delete gestureFilter;
}

void EventGenerator::addGesture(QjtMouseGesture * gesture)
{
    gestureFilter->addGesture(gesture);
}

void EventGenerator::clearGestures()
{
    gestureFilter->clearGestures();
}

// will be executed when a new calibration point data is received
void EventGenerator::processInputData(QPoint newPoint, int i,int type,int visibleCount)
{
    newPoint = applySmoothing(newPoint);
    int mouseButton = Button1;  // the left mouse button is Button1

    // TODO check upper bounds for points as well?
    if(newPoint.x() < 0) newPoint.setX(0);
    if(newPoint.y() < 0) newPoint.setY(0);

    // any more points than 2 is equivalent to 2
    if(visibleCount > 2)
        visibleCount = 2;

    // for now, we have states for 0, 1 or 2 points
    if(visibleCount == 2) {
        // we always get 1 visible point first, than 2
        // finalize previous mouse event (release button)
        // TODO find a way to undo the mouse event..?
        if(prev_visibleCount == 1) {
            mouseRelease(mouseButton, newPoint);
            gestureFilter->gestureStart(newPoint);
        } else if(prev_visibleCount == 2) {
            gestureFilter->gestureMove(newPoint);
        }

    } else if(visibleCount <= 1) {
        // TODO cancel/execute any pending gesture events
        // TODO should no mouse event generation while exiting from 2 points
        // generate a mouse event
        if(prev_visibleCount == 2) {
            // tell the gesture system that the gesture is finalized
            gestureFilter->gestureEnd(newPoint);
        }
        switch(type) {
            case MOUSE_PRESSED:
                mousePress(mouseButton,newPoint);
                break;
            case MOUSE_MOVE:
                mouseMove(mouseButton,newPoint);
                break;
            case MOUSE_RELEASED:
                mouseRelease(mouseButton,newPoint);
                break;
        }
    }

    updateState(newPoint, i, type, visibleCount);
}

void EventGenerator::updateState(QPoint pt,int i,int type,int visibleCount)
{
    prev_point = pt;
    prev_i = i;
    prev_type = type;
    prev_visibleCount = visibleCount;
}

QPoint EventGenerator::applySmoothing(QPoint inputPoint)
{
    qreal newx = inputPoint.x();
    qreal newy = inputPoint.y();

    // perform smoothing through averaging
    if(smoothCounter < 10 && !full) { // ilk 10 nokta
        smoothPoints[smoothCounter] = QPoint(newx,newy);
        smoothCounter++;
        newx = 0;
        newy = 0;
        for(int i = 0 ; i < smoothCounter ; i++) {
            newx += smoothPoints[i].x();
            newy += smoothPoints[i].y();
        }

        newx /= smoothCounter;
        newy /= smoothCounter;
    }
    else if(smoothCounter < 10 && full) {
        smoothPoints[smoothCounter] = QPoint(newx,newy);
        smoothCounter++;
        newx = 0;
        newy = 0;
        for(int i = 0 ; i < 10 ; i++) {
            newx += smoothPoints[i].x();
            newy += smoothPoints[i].y();
        }

        newx /= 10;
        newy /= 10;
    }
    else if(smoothCounter >= 10){
        full = true;
        smoothCounter = 0;

        smoothPoints[smoothCounter] = QPoint(newx,newy);
        smoothCounter++;
        newx = 0;
        newy = 0;
        for(int i = 0 ; i < 10 ; i++) {
            newx += smoothPoints[i].x();
            newy += smoothPoints[i].y();
        }

        newx /= 10;
        newy /= 10;
    }

    return QPoint(newx, newy);
}


// TODO find the source code for the XTest.. things and use it directly here if possible
void EventGenerator::mousePress(int button,QPoint p)
{
    if(!isMouseButtonDown) {
        qWarning() << "Pressed!" << button << p;

        XTestFakeMotionEvent (dpy, 0, p.x(), p.y(), CurrentTime);
        XTestFakeButtonEvent (dpy, button, True,  CurrentTime);
        XSync(dpy, 0);

        isMouseButtonDown = true;
    } else
        qWarning() << "ignoring mousePress";
}

void EventGenerator::mouseMove(int button, QPoint p)
{
    qWarning() << "Move!" << button << p;

    if(!isMouseButtonDown)
        mousePress(button, p);

    XTestFakeMotionEvent (dpy, 0, p.x(), p.y(), CurrentTime);
    XSync(dpy, 0);

}


void EventGenerator::mouseRelease(int button,QPoint p)
{
    if(isMouseButtonDown) {
        qWarning() << "Released!" << button << p;

        XTestFakeMotionEvent (dpy, 0, p.x(), p.y(), CurrentTime);
        XTestFakeButtonEvent (dpy, button, False,  CurrentTime);
        XSync(dpy,0);

        // reset the smoothing points
        smoothCounter = 0;
        full = false;
        for(int i = 0; i < 10; i++) {
            smoothPoints[i] = QPoint(0,0);
        }

        isMouseButtonDown = false;
    } else
        qWarning() << "ignoring mouseRelease";
}
