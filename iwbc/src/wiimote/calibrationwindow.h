/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QDesktopWidget>

#include <QPainter>
#include <QEvent>
#include <QCoreApplication>
#include <QDesktopWidget>

#include <stdio.h>
#include <stdlib.h>
#include <QWaitCondition>
#include <QMutex>

#ifndef WIN32
        #include <unistd.h>
#endif
#include "basedrawingwidget.h"
#include "wiiuse.h"
#include "inputcalibration.h"
#include "irthread.h"


#define HEIGHT_FIX  20  // TODO set this according to platform
#define MAX_WIIMOTES    1
#define CLICK_POINT_THRESHOLD 50 // minimum Manhattan spacing for calibration points


namespace Ui {
    class CalibrationWindow;
}

class CalibrationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibrationWindow(QWidget *parent = 0);
    ~CalibrationWindow();

    void setCalibrationPointImages(QImage newImageInitial, QImage newImageTouched);
    void recalibrate();

    QWaitCondition wc;
    QMutex mutex;
    int calibrationPointTouchCount;

private:
    Ui::CalibrationWindow *ui;
    QPixmap calibrationPointImageInitial;
    QPixmap calibrationPointImageTouched;
    InputCalibration mapper;
    int calibrationPointWidth, calibrationPointHeight;

    QPoint prevPoint;
    BaseDrawingWidget *draw;
    IRThread *receiver;


    void repositionItems();
    void setCalibrationPointTouchStatus(int touchedCount);

protected:
    bool event(QEvent *event);

public slots:
    void inputReceived(QPoint * irpoints,int i,int type,int visibleCount);
    void calibrationPointReceived(QPoint p);
    void connected();
    void calibrationComplete();

signals:
    void initComplete();

private slots:
    void on_acceptSettings_clicked();
};

#endif // CALIBRATIONWINDOW_H
