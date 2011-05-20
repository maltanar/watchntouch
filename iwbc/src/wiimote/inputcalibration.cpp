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
QPointF * InputCalibration::getCalibrationPoints()
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

        emit calibrationComplete();

        isCalibrated = true;
        pointCounter = 0;

        QTransform t;
        QPolygonF one(4);
        QPolygonF two(4);

        one.clear();
        two.clear();

        // state the polygons that are to be transformed - from one ---> two
        one << calibrationPoints[0] << calibrationPoints[1] << calibrationPoints[2] << calibrationPoints[3];
        two << QPointF(0.0,0.0) << QPointF(screenWidth,0.0) << QPointF(screenWidth,screenHeight) <<  QPointF(0.0,screenHeight);        

        // set transformation affine
        transform.toAffine();
        bool wtf = t.quadToQuad(one,two,transform);
        qWarning() << "TRANSFORM operation is " << wtf;

    }
}

// map from Wiimote coordinates to screen coordinates
// according to current calibration information
QPoint InputCalibration::mapFromWiimoteToScreen(QPoint inputPoint)
{
    // if calibration was not performed, return the same point

    if(!isCalibrated)
        return inputPoint;

    qreal x = inputPoint.x();
    qreal y = inputPoint.y();

    qreal newx,newy;
    transform.map(x,y,&newx,&newy);

    //qWarning() << "GELEN: " << x << y << " GIDEN " << newx << newy;

    return QPoint(newx,newy);
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
