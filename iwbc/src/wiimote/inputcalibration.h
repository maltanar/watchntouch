#ifndef INPUTCALIBRATION_H
#define INPUTCALIBRATION_H

// The InputCalibration class has two main responsibilities:
// - display the calibration interface
// - map received IR points to screen coordinates

// calibration point vs calibration sample:
// a calibration point is one of the 4 points we use for calibration
// a calibration sample is a point that helps to calculate the exact position of a calibration point

#include <QObject>
#include <QPoint>
#include <QTransform>

#define NUM_CALIBRATION_POINTS      4
#define NUM_SAMPLES_PER_POINT       1   // WARNING if this is >1 the threshold thing must be removed
#define CALIBRATION_POINT_THRESHOLD 400 // minimum Manhattan spacing for calibration points

class InputCalibration : public QObject
{
    Q_OBJECT

public:
    InputCalibration();
    void setCalibrationPoints(QPoint *newPoints);
    QPointF *getCalibrationPoints();
    void addCalibrationSample(QPoint newPoint);
    bool calibrated();

    QPoint* mapFromWiimoteToScreen(QPoint * inputPoint);
    void processWiimotePoint(QPoint inputPoint);

    void recalibrate();
    double getCalibratedDistance(QPoint p1, QPoint p2, QPoint p3);


private:
    QTransform transform;
    QPointF calibrationPoints[NUM_CALIBRATION_POINTS];
    QPoint sampleAverage;
    QPoint prevSample;
    int screenWidth, screenHeight;
    bool isCalibrated;
    int sampleCounterForCurrentPoint;
    int pointCounter;

signals:
    void calibrationPointReceived(QPoint p);
    void calibrationComplete();

};

#endif // INPUTCALIBRATION_H
