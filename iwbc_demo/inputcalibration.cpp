#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <math.h>

#include "inputcalibration.h"

// class constructor
// - get screen width/height and store them
InputCalibration::InputCalibration()
{
    QDesktopWidget * desktop = QApplication::desktop();
    QRect rect = desktop->screenGeometry(-1);
    screenWidth = rect.width();
    screenHeight = rect.height();

    isCalibrated = false;

    pointCounter = 0;
    sampleCounterForCurrentPoint = 0;
    smoothCounter = 0;
    full = false;
}

// specify a new set of calibration points
// resets sampled point adding!
void InputCalibration::setCalibrationPoints(QPoint *newPoints)
{
    pointCounter = 0;
    calibrationPoints[0] = newPoints[0];
    calibrationPoints[1] = newPoints[1];
    calibrationPoints[2] = newPoints[2];
    calibrationPoints[3] = newPoints[3];

    isCalibrated = true;
}

// have we received calibration info yet?
bool InputCalibration::calibrated()
{
    return isCalibrated;
}

// get the existing calibration points
QPoint * InputCalibration::getCalibrationPoints()
{
    return calibrationPoints;
}

// add a data point sample for calibration
// once we have NUM_SAMPLES_PER_POINT samples, get the new point
void InputCalibration::addCalibrationSample(QPoint newPoint)
{
    // TODO the following conditional ignores repeated press events during calibration
    // this should be handled with TouchBegin - TouchEnd instead
    // but we may want to keep the following for extra security...just in case
    qWarning() << "points:" << newPoint << prevSample << "samplecounter" << sampleCounterForCurrentPoint;
    if((prevSample-newPoint).manhattanLength() < CALIBRATION_POINT_THRESHOLD && prevSample != QPoint(0,0))
    {
        qWarning() << "ignoring repeated calibration sample";
        return;
    }
    else
        qWarning() << "actually received new calibration sample";


    // sum with existing values
    //sampleAverage += sampleAverage + newPoint;   BURASINI DEĞİŞTİRİYORUM!
    sampleAverage += newPoint;
    sampleCounterForCurrentPoint++;
    prevSample = newPoint;

    if(sampleCounterForCurrentPoint == NUM_SAMPLES_PER_POINT) {
        // we have enough samples for this point now
        // get the average from the data sum
        sampleAverage /= NUM_SAMPLES_PER_POINT;
        // we have one more calibration point now
        calibrationPoints[pointCounter] = sampleAverage;
        emit calibrationPointReceived(sampleAverage);
        pointCounter++;
        // get ready for the new set of samples
        sampleAverage = QPoint(0,0);
        sampleCounterForCurrentPoint = 0;
    }

    if(pointCounter == NUM_CALIBRATION_POINTS) {
        // we have all the calibration points we need

        // calculate new x-y coordinates average width and height, use them in the linear transformation. CAUTION! coming x's are y's for us, and y's are x's!
        calibratedHeight = (sqrt((calibrationPoints[0].x() - calibrationPoints[1].x())*(calibrationPoints[0].x() - calibrationPoints[1].x()) + (calibrationPoints[0].y() - calibrationPoints[1].y())*(calibrationPoints[0].y() - calibrationPoints[1].y()))
                            + sqrt((calibrationPoints[3].x() - calibrationPoints[2].x())*(calibrationPoints[3].x() - calibrationPoints[2].x()) + (calibrationPoints[3].y() - calibrationPoints[2].y())*(calibrationPoints[3].y() - calibrationPoints[2].y()))) / 2;
        calibratedWidth =  (sqrt((calibrationPoints[0].x() - calibrationPoints[3].x())*(calibrationPoints[0].x() - calibrationPoints[3].x()) + (calibrationPoints[0].y() - calibrationPoints[3].y())*(calibrationPoints[0].y() - calibrationPoints[3].y()))
                            + sqrt((calibrationPoints[1].x() - calibrationPoints[2].x())*(calibrationPoints[1].x() - calibrationPoints[2].x()) + (calibrationPoints[1].y() - calibrationPoints[2].y())*(calibrationPoints[1].y() - calibrationPoints[2].y()))) / 2 ;

        emit calibrationComplete();

        isCalibrated = true;
        pointCounter = 0;
    }
}

double InputCalibration::getCalibratedDistance(QPoint p1, QPoint p2, QPoint p3) {

    double m,n,distance1,distance2;
    int x = p3.x();
    int y = p3.y();

    // line of points 0 and 3, y = mx + n

    m = (p1.y() - p2.y()) / (p1.x() - p2.x());
    n = p1.y() - m * p1.x();

    // distance btwn x,y to the found line. ax + by + c / sqrt(a*a + b*b); here we have y = mx + n --> mx + n - y = 0;

    distance1 =  fabs( (m * x + (-1) * y + n) ) / sqrt(m * m + 1);

    distance2 = sqrt( (p1.x() - x)*(p1.x() - x) + (p1.y() - y)*(p1.y() - y) );

    return sqrt(distance2*distance2 - distance1*distance1);

}


// map from Wiimote coordinates to screen coordinates
// according to current calibration information
QPoint InputCalibration::mapFromWiimoteToScreen(QPoint inputPoint)
{
    // if calibration was not performed, return the same point
    // TODO transformation ı duzelt, sadece ust ve sag kenar degil dort kenari da kullanır hale getir.
    if(!isCalibrated)
        return inputPoint;

    int x = inputPoint.x();
    int y = inputPoint.y();

    if(smoothCounter < 10 && !full) {
        smoothPoints[smoothCounter] = QPoint(x,y);
        smoothCounter++;
        for(int i = 0 ; i < smoothCounter ; i++) {
            x += smoothPoints[i].x();
            y += smoothPoints[i].y();
        }

        x /= smoothCounter;
        y /= smoothCounter;
    }
    if(smoothCounter < 10 && full) {
        smoothPoints[smoothCounter] = QPoint(x,y);
        smoothCounter++;

        for(int i = 0 ; i < 10 ; i++) {
            x += smoothPoints[i].x();
            y += smoothPoints[i].y();
        }

        x /= 10;
        y /= 10;
    }
    else if(smoothCounter >= 10){
        full = true;
        smoothCounter = 0;

        smoothPoints[smoothCounter] = QPoint(x,y);
        smoothCounter++;
        for(int i = 0 ; i < 10 ; i++) {
            x += smoothPoints[i].x();
            y += smoothPoints[i].y();
        }

        x /= 10;
        y /= 10;
    }

    //printf("gelen point %d %d\n",x,y);

    double calibratedDistWidthLeft,calibratedDistWidthRight;
    double calibratedDistHeightUp,calibratedDistHeightDown;
    double calibratedDistWidth,calibratedDistHeight;

    calibratedDistWidthLeft = getCalibratedDistance(QPoint(calibrationPoints[0]),QPoint(calibrationPoints[3]),QPoint(x,y));
    calibratedDistWidthRight = getCalibratedDistance(QPoint(calibrationPoints[1]),QPoint(calibrationPoints[2]),QPoint(x,y));

    calibratedDistWidth = (calibratedDistWidthLeft + calibratedDistWidthRight) / 2;

    calibratedDistHeightUp = getCalibratedDistance(QPoint(calibrationPoints[0]),QPoint(calibrationPoints[1]),QPoint(x,y));
    calibratedDistHeightDown = getCalibratedDistance(QPoint(calibrationPoints[3]),QPoint(calibrationPoints[2]),QPoint(x,y));

    calibratedDistHeight = (calibratedDistHeightUp + calibratedDistHeightDown) / 2;

    //m = (calibrationPoints[0].x() + calibrationPoints[1].x()) / 2.0 - x;
    //n = y - (calibrationPoints[0].y() + calibrationPoints[3].y()) / 2.0;

    x = (calibratedDistHeight/calibratedHeight) * screenWidth;
    y = (calibratedDistWidth/calibratedWidth) * screenHeight;


    //printf("giden point %d %d\n",x,y);
    //x = ((x - calibrationPoints[3].x()) * screenWidth)  / (double)(calibrationPoints[2].x() - calibrationPoints[3].x()) ;
    //y = (((y - calibrationPoints[0].y() )* screenHeight) / (double)( calibrationPoints[3].y() - calibrationPoints[0].y())) ;

    return QPoint(x,y);
}

void InputCalibration::processWiimotePoint(QPoint inputPoint)
{
    QPoint mappedPoint = mapFromWiimoteToScreen(inputPoint);

    // TOOD send point to event generator
}

void InputCalibration::recalibrate()
{
    // TODO here we will display the calibration UI and perform a new calibration
}
