#include "appglobals.h"
#include "eventgenerator.h"

EventGenerator::EventGenerator(QObject *parent) :
    QObject(parent)
{
    dpy = XOpenDisplay(NULL);
    full = false;
    isMouseButtonDown = false;
    smoothCounter = 0;
    m_timeoutFlag = false;
    m_visibleCount = 0;

    for(int i = 0 ; i < 10; i++) {
        smoothPoints[i] = QPoint(0,0);
    }

    m_currentState = STATE_NOTHING;

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

EventGenerator::~EventGenerator()
{
    XCloseDisplay(dpy);
}

void EventGenerator::handleStateChange()
{
    //TODO

    qWarning() << "prevstate" << m_currentState;

    if(m_currentState == STATE_NOTHING && m_visibleCount == 1) {
        // start timer and move to buffering state
        m_currentState = STATE_FIRST_VISIBLE;
        m_timer.start(GESTURE_START_DELAY);
    } else if(m_currentState == STATE_FIRST_VISIBLE && m_timeoutFlag) {
        // we are no longer accepting gestures
        // start regular drawing
        m_currentState = STATE_NORMAL_DRAW;
        m_timeoutFlag = false;
    } else if(m_currentState == STATE_FIRST_VISIBLE && m_visibleCount == 0) {
        m_currentState = STATE_DRAW_RELEASE;
    } else if(m_currentState == STATE_NORMAL_DRAW && m_visibleCount == 0) {
        // stop drawing
        m_currentState = STATE_DRAW_RELEASE;
    } else if(m_currentState == STATE_DRAW_RELEASE) {
        // go back to initial state
        m_currentState = STATE_NOTHING;
    }

    qWarning() << "currentsate" << m_currentState;
}

// will be executed when a new calibration point data is received
void EventGenerator::processInputData(QPoint *newpoints, int i,int type,int visibleCount)
{
    //newpoints[0] = applySmoothing(newpoints[0]);

    for(int i = 0; i < 4; i++) {
        if(newpoints[i].x() < 0)
            newpoints[i].setX(0);
        if(newpoints[i].y() < 0)
            newpoints[i].setY(0);
    }

    // TODO handle state changes first!
    m_visibleCount = visibleCount;
    handleStateChange();

    switch(m_currentState) {
        case STATE_FIRST_VISIBLE:
            // don't generate any events, buffer incoming points
            // TODO check for mouse release at this stage and generate buffered events if needed
            m_pointBuffer.append(newpoints[0]);
            break;
        case  STATE_NORMAL_DRAW:
            // generate mouse events for buffered points
            if(m_pointBuffer.size() > 0) {
                for(int i=0; i < m_pointBuffer.count(); i++) {
                    if(i == 0)
                        mousePress(Button1, m_pointBuffer.at(i));
                    else
                        mouseMove(Button1, m_pointBuffer.at(i));
                }
                m_pointBuffer.clear();
            }
            // generate mouse events
            mouseMove(Button1, newpoints[0]);
            break;
        case STATE_DRAW_RELEASE:
            if(m_pointBuffer.size() > 0) {
                for(int i=0; i < m_pointBuffer.count(); i++) {
                    if(i == 0)
                        mousePress(Button1, m_pointBuffer.at(i));
                    else
                        mouseMove(Button1, m_pointBuffer.at(i));
                }
                m_pointBuffer.clear();
            }
            mouseRelease(Button1, newpoints[0]);
            break;
        case  STATE_SECOND_VISIBLE:
            break;
        case  STATE_GESTURING:
            break;
        case  STATE_SINGLE_IGNORE:
            break;
    }

}

void EventGenerator:: timeout()
{
    // TODO handle state change
    m_timer.stop();
    m_timeoutFlag = true;
    handleStateChange();
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
