/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

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
        m_timer.stop();
        m_timeoutFlag = false;
    } else if(m_currentState == STATE_NORMAL_DRAW && m_visibleCount == 0) {
        // stop drawing
        m_currentState = STATE_DRAW_RELEASE;
    } else if(m_currentState == STATE_DRAW_RELEASE) {
        // go back to initial state
        m_currentState = STATE_NOTHING;
    } else if(m_currentState == STATE_FIRST_VISIBLE && m_visibleCount == 2) {
        // discard buffer
        m_currentState = STATE_SECOND_VISIBLE;
        m_timer.stop();
        m_timeoutFlag = false;
    } else if(m_currentState == STATE_SECOND_VISIBLE) {
        // start gesturing
        m_currentState = STATE_GESTURING;
    } else if(m_currentState == STATE_GESTURING && m_visibleCount == 1) {
        // finish the gesture and trigger it
        m_currentState = STATE_FINISH_GESTURING;
    } else if(m_currentState == STATE_FINISH_GESTURING) {
        // ignore coming single point events after gesture
        if(m_visibleCount == 0)
            m_currentState = STATE_NOTHING;
        else if(m_visibleCount == 1)
            m_currentState = STATE_SINGLE_IGNORE;

    } else if(m_currentState == STATE_SINGLE_IGNORE && m_visibleCount == 0) {
        // go back to initial state
        m_currentState = STATE_NOTHING;
    } else if(m_currentState == STATE_SINGLE_IGNORE && m_visibleCount == 2) {
        // go back to initial state
        m_currentState = STATE_NOTHING;
    }
    else if(m_currentState == STATE_NOTHING && m_visibleCount == 2) {
        // if directly two points come
        m_currentState = STATE_SECOND_VISIBLE;
    } else if(m_currentState == STATE_GESTURING && m_visibleCount == 0) {
        // go back to initial state
        m_currentState = STATE_FINISH_GESTURING;
    } else if(m_currentState == STATE_SECOND_VISIBLE && m_visibleCount == 1) {
        // indeed, this is an unexpected condition, but we have seen it
        m_currentState == STATE_FIRST_VISIBLE;
    }

    qWarning() << "currentsate" << m_currentState;
}

// will be executed when a new calibration point data is received
void EventGenerator::processInputData(QPoint *newpoints, int i,int type,int visibleCount)
{
    //newpoints[0] = applySmoothing(newpoints[0]);

    qWarning() << "newpoint[0]" << newpoints[0];
    for(int i = 0; i < 4; i++) {
        if(newpoints[i].x() < 0)
            newpoints[i].setX(0);
        if(newpoints[i].y() < 0)
            newpoints[i].setY(0);
    }

    qWarning() << "visibleCount:" << visibleCount;

    // TODO handle state changes first!
    m_visibleCount = visibleCount;

    qWarning() << "new point" << newpoints[0];
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
            // set the first received point as the gesture start point
            m_gestureStartPoint = newpoints[0];
            m_pointBuffer.clear();
            m_firstPreviousPoint = newpoints[0];
            m_secondPreviousPoint = newpoints[1];
            break;
        case  STATE_GESTURING:
            recognizeGesture(newpoints);
            break;
        case  STATE_FINISH_GESTURING:
            finishGesture();
            break;
        case  STATE_SINGLE_IGNORE:
            // just ignore coming events
            break;
        default:
            qWarning() << "no state!" << visibleCount;

    }

}

void EventGenerator::recognizeGesture(QPoint *newpoints)
{
    // depends on the context
    // pinch and swipe can be written without this knowledge, since it requires buffering
    // however, scrolling will be done directly applying coming points to the scroll
    // so, it will be written in comment, without trying it.

    qWarning() << "recognizeGesture ------------------------------------------";

    QPoint firstDiff(newpoints[0].x() - m_firstPreviousPoint.x(),newpoints[0].y() - m_firstPreviousPoint.y());
    QPoint secondDiff(newpoints[1].x() - m_secondPreviousPoint.x(),newpoints[1].y() - m_secondPreviousPoint.y());

    if(qAbs(firstDiff.y() - secondDiff.y()) < VERTICAL_SWIPE_SENSITIVITY)
        emit swipeGesture(GESTURE_DIR_DOWN, 0, -firstDiff.y());

    qWarning() << "first prev point:" << m_firstPreviousPoint;
    qWarning() << "newpoints[0]:" << newpoints[0];
    qWarning() << "first Diff:" << firstDiff;
    qWarning() << "second prev point:" << m_secondPreviousPoint;
    qWarning() << "newpoints[1]:" << newpoints[1];
    qWarning() << "second Diff:" << secondDiff;

    if(qAbs(firstDiff.x()) < 20)
        m_firstPointDifferenceBuffer.append(firstDiff);
    if(qAbs(secondDiff.x()) < 20)
        m_secondPointDifferenceBuffer.append(secondDiff);

    m_firstPreviousPoint = newpoints[0];
    m_secondPreviousPoint = newpoints[1];
}

void EventGenerator::finishGesture()
{
    // this method will determine the gesture by using the buffer
    // then it will clear the buffer

    int firstLen = m_firstPointDifferenceBuffer.size();
    int secondLen = m_secondPointDifferenceBuffer.size();

    long firstXSum = 0;
    long firstYSum = 0;
    long secondXSum = 0;
    long secondYSum = 0;

    for(int i = 0; i < firstLen; i++) {
        qWarning() << "first diff at" << i << ":" << m_firstPointDifferenceBuffer.at(i);
        firstXSum += m_firstPointDifferenceBuffer.at(i).x();
        firstYSum += m_firstPointDifferenceBuffer.at(i).y();
    }

    for(int i = 0; i < secondLen; i++) {
        qWarning() << "second diff at" << i << ":" << m_secondPointDifferenceBuffer.at(i);
        secondXSum += m_secondPointDifferenceBuffer.at(i).x();
        secondYSum += m_secondPointDifferenceBuffer.at(i).y();
    }

    /*
    int sameXSign = firstXSum > 0 && secondXSum > 0 || firstXSum < 0 && secondXSum < 0 || firstXSum == 0 && secondXSum == 0;
    int sameYSign = firstYSum > 0 && secondYSum > 0 || firstYSum < 0 && secondYSum < 0 || firstYSum == 0 && secondYSum == 0;

    if(sameXSign && sameYSign) {
        qWarning() << "both X and Y differences have same sign, trigger swipe gesture if context is appropriate";
        qWarning() << "firsXSum:" << firstXSum;
        qWarning() << "secondXSum:" << secondXSum;
        qWarning() << "firstYSum:" << firstYSum;
        qWarning() << "secondYSum:" << secondYSum;
        qWarning() << "sameXSign:" << sameXSign;
        qWarning() << "sameYSign:" << sameYSign;
    }
    else if(!sameXSign || !sameYSign) {
        qWarning() << "at least one of X or Y sign is different, trigger pinch gesture";
        qWarning() << "firsXSum:" << firstXSum;
        qWarning() << "secondXSum:" << secondXSum;
        qWarning() << "firstYSum:" << firstYSum;
        qWarning() << "secondYSum:" << secondYSum;
        qWarning() << "sameXSign:" << sameXSign;
        qWarning() << "sameYSign:" << sameYSign;
    }
    */

    qWarning() << "firsXSum:" << firstXSum;
    qWarning() << "secondXSum:" << secondXSum;
    qWarning() << "firstYSum:" << firstYSum;
    qWarning() << "secondYSum:" << secondYSum;

    if(firstXSum > 0 && secondXSum > 0) {
        qWarning() << "both x signs' are same and positive, if presentation swipe right";
        emit swipeGesture(GESTURE_DIR_RIGHT, 0, 0);
    }
    else if(firstXSum < 0 && secondXSum < 0) {
        qWarning() << "both x signs' are same and negative, if presentation swipe left";
        emit swipeGesture(GESTURE_DIR_LEFT, 0, 0);
    }
    else if(firstXSum < 0 && secondXSum > 0 || firstXSum > 0 && secondXSum < 0) {
        // TODO pinch in/out nasıl ayrılır?
        if(qAbs(firstXSum) > PINCH_THRESHOLD || qAbs(secondXSum) > PINCH_THRESHOLD) {
            qWarning() << "open context menu" << firstXSum << secondYSum;
            emit pinchGesture(m_gestureStartPoint, false);
        } else
            qWarning() << "don't open context menu (too short)" << firstXSum << secondYSum;
    }

    if(firstYSum > 0 && secondYSum > 0) {
        // TODO scrolling will be done directly in recognizeGesture() function
        // This is just to check to capability of capturing y direction changes
        qWarning() << "both y signs' are same and negative, if web_page and read_only swipe scroll down";
    }
    else if(firstYSum < 0 && secondYSum < 0) {
        qWarning() << "both y signs' are same and positive, if web_page and read_only swipe scroll up";
    }

    m_firstPointDifferenceBuffer.clear();
    m_secondPointDifferenceBuffer.clear();

    handleStateChange();
}


void EventGenerator:: timeout()
{
    // TODO handle state change
    qWarning() << "timeout !!";
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
