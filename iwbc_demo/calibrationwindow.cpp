#include <QDebug>
#include <QPainter>
#include <QEvent>
#include <QCoreApplication>
#include <QDesktopWidget>

#include "ui_calibrationwindow.h"
#include "calibrationwindow.h"

#include <X11/extensions/XTest.h>

CalibrationWindow::CalibrationWindow(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::CalibrationWindow)
{
    // UI
    ui->setupUi(this);
    draw = NULL;

    // set calibration point width and height to default
    calibrationPointWidth = calibrationPointHeight = 100;
    calibrationPointTouchCount = 0;

    ui->acceptSettings->hide();

    QImage initial, touched;
    initial.load(":/images/touchpointred.png");
    touched.load(":/images/touchpointgreen.png");

    setCalibrationPointImages(initial, touched);

    connect(&mapper, SIGNAL(calibrationPointReceived(QPoint)),this,SLOT(calibrationPointReceived(QPoint)));
    connect(&mapper, SIGNAL(calibrationComplete()), this, SLOT(calibrationComplete()));

    //setCalibrationPointTouchStatus(-1); // hide all calibration points


    // show connection instructions in fullscreen
    ui->instructions->move(0,0);
    ui->instructions->resize(qApp->desktop()->size());
    ui->instructions->raise();

    QImage instructions;
    instructions.load(":/images/connection.png");
    instructions = instructions.scaled(ui->instructions->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->calibrationPoint1->setVisible(false);

    ui->instructions->setPixmap(QPixmap::fromImage(instructions));

    receiver = new IRThread();  // create input receiver thread

    QObject::connect(receiver,SIGNAL(IRInputReceived(int,int,int,int,int)),this,SLOT(inputReceived(int,int,int,int,int)));
    QObject::connect(receiver,SIGNAL(connected()),this,SLOT(connected()));

    receiver->start();

    dpy = XOpenDisplay(NULL);

}

void CalibrationWindow::connected()
{
    // wiimote is now connected
    // hide instructions
    ui->instructions->setVisible(false);
    // show first calibration point
    ui->calibrationPoint1->setVisible(true);
    // load calibration instructions image
    QImage instructions;
    instructions.load(":/images/calibration_instruct.png");
    instructions = instructions.scaled(ui->instructions->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->instructions->setPixmap(QPixmap::fromImage(instructions));
    ui->instructions->lower();
    // show instructions again
    ui->instructions->setVisible(true);
}


CalibrationWindow::~CalibrationWindow()
{
    XCloseDisplay(dpy);
    delete ui;
}

// set a new calibration point image
// this image will be displayed as the calibration point
void CalibrationWindow::setCalibrationPointImages(QImage newImageInitial, QImage newImageTouched)
{
    // check if image sizes match
    if(newImageInitial.size() != newImageTouched.size()) {
        qWarning() << "error: setCalibrationPointImages(): initial and touched image sizes do not match! ignoring changes";
        qWarning() << "provided sizes are: initial: " << newImageInitial.size() << " touched: " << newImageTouched.size();
        return;
    }

    // store the new images inside class
    calibrationPointImageInitial = QPixmap::fromImage(newImageInitial);
    calibrationPointImageTouched = QPixmap::fromImage(newImageTouched);

    // get dimensions of new image
    calibrationPointWidth = newImageInitial.width();
    calibrationPointHeight = newImageInitial.height();
    qWarning() << "info: new calibration point image width=" << calibrationPointWidth << "height=" << calibrationPointHeight;

    // reset status of all calibration points
    repositionItems(); // this is needed in case image size has changed
    setCalibrationPointTouchStatus(0);

}

// repositions the widgets (the calibration points etc.) according to the
// current size of the screen
void CalibrationWindow::repositionItems()
{
    int width = this->size().width();
    int height = this->size().height();

    // reposition calibration point 1 to upper left hand corner
    ui->calibrationPoint1->move(0, 0);

    // reposition calibration point 2 to upper right hand corner
    ui->calibrationPoint2->move(width - calibrationPointWidth,0);

    // reposition calibration point 3 to lower right hand corner
    ui->calibrationPoint3->move(width - calibrationPointWidth, height - calibrationPointHeight - HEIGHT_FIX);

    // reposition calibration point 4 to lower left hand corner
    ui->calibrationPoint4->move(0, height - calibrationPointHeight  - HEIGHT_FIX);

    // reposition accept calibration button to screen center
    ui->acceptSettings->move((width-ui->acceptSettings->width())/2,(height-ui->acceptSettings->height())/2 );
}

// sets the status of the calibration points
// the number of points given in touchedCount turn "green" (touched image)
// other ones are set to "red" (initial image)
void CalibrationWindow::setCalibrationPointTouchStatus(int touchedCount)
{
    /*if(touchedCount < 0) {
        // special meaning: hide all calibration points
        ui->calibrationPoint1->setVisible(false);
        ui->calibrationPoint2->setVisible(false);
        ui->calibrationPoint3->setVisible(false);
        ui->calibrationPoint4->setVisible(false);
    }*/

    qWarning() << "touchedCount: " << touchedCount << "internal:" << calibrationPointTouchCount;
    ui->calibrationPoint1->setPixmap(touchedCount > 0 ? calibrationPointImageTouched : calibrationPointImageInitial);
    ui->calibrationPoint2->setPixmap(touchedCount > 1 ? calibrationPointImageTouched : calibrationPointImageInitial);
    ui->calibrationPoint3->setPixmap(touchedCount > 2 ? calibrationPointImageTouched : calibrationPointImageInitial);
    ui->calibrationPoint4->setPixmap(touchedCount > 3 ? calibrationPointImageTouched : calibrationPointImageInitial);

    ui->calibrationPoint1->setVisible(touchedCount > -1 ? true : false);
    ui->calibrationPoint2->setVisible(touchedCount > 0 ? true : false);
    ui->calibrationPoint3->setVisible(touchedCount > 1 ? true : false);
    ui->calibrationPoint4->setVisible(touchedCount > 2 ? true : false);

    calibrationPointTouchCount = touchedCount;

    // TODO show "calibration is complete" screen when four points are received
}

void CalibrationWindow::mousePress(int button,QPoint p) {

    qWarning() << "Pressed!" << button << p;

    XTestFakeMotionEvent (dpy, 0, p.x(), p.y(), CurrentTime);
    XTestFakeButtonEvent (dpy, button, True,  CurrentTime);
    XSync(dpy, 0);
}

void CalibrationWindow::mouseRelease(int button,QPoint p) {

    qWarning() << "Released!" << button << p;

    XTestFakeMotionEvent (dpy, 0, p.x(), p.y(), CurrentTime);
    XTestFakeButtonEvent (dpy, button, False,  CurrentTime);
    XSync(dpy,0);
}

void CalibrationWindow::mouseMove(int button, QPoint p)
{
    qWarning() << "Move!" << button << p;

    XTestFakeMotionEvent (dpy, 0, p.x(), p.y(), CurrentTime);
    XSync(dpy, 0);
}



// will be executed when a new calibration point data is received
void CalibrationWindow::inputReceived(int x,int y,int i,int type,int visibleCount)
{
    // TODO for calibration, we should gather multiple (4-5) datapoints for each calibration point and get their avg
    if(!mapper.calibrated() && i == 0) {
        mapper.addCalibrationSample(QPoint(x,y));
    }
    else if(mapper.calibrated()) {
        // TODO the events should be generated by EventGenerator
        QPoint newPoint = mapper.mapFromWiimoteToScreen(QPoint(x,y));
        // TODO check upper bounds for points as well?
        if(newPoint.x() < 0) newPoint.setX(0);
        if(newPoint.y() < 0) newPoint.setY(0);
        // TODO i will not always be 0, handle this properly
        // gesture workaround: if visiblecount > 1, right button down, else left button down
        //int button = (visibleCount > 1 ? Button3 : Button1); TODO fix 1->2 visible point transition
        int button = Button1;
        switch(type) {
        case MOUSE_PRESSED:
            mousePress(button,newPoint);
            break;
        case MOUSE_MOVE:
            mouseMove(button,newPoint);
            break;
        case MOUSE_RELEASED:
            mouseRelease(button,newPoint);
            break;
        }
    }
}

// perform the calibration operation from start by setting the touch count to zero
void CalibrationWindow::recalibrate()
{
    setCalibrationPointTouchStatus(0);
    mapper.recalibrate();
}


// handle resize events for the window - when resized, reposition calibration points
bool CalibrationWindow::event(QEvent *event)
{
    if(event->type() == QEvent::Resize)
        repositionItems();

    return QDialog::event(event);
}

void CalibrationWindow::calibrationPointReceived(QPoint p)
{
    setCalibrationPointTouchStatus(calibrationPointTouchCount+1);
}

void CalibrationWindow::calibrationComplete()
{
    qWarning() << "calibration is now completed";
    // hide all calibration points
    setCalibrationPointTouchStatus(-1);
    // hide instructions
    ui->instructions->hide();
    // show accept button
    ui->acceptSettings->show();

    return;

    // calibration is complete, create drawing area if it does not exist
    // TODO add option for recalibration and re-enable
    if(!draw) {
        qWarning() << "leleloooy looy loy";
        draw = new BaseDrawingWidget(this);
        draw->setGeometry(QRect(0,0, width(), height()));
        draw->raise();
        draw->show();
    }


}

void CalibrationWindow::on_acceptSettings_clicked()
{
    emit initComplete();
    hide();
}
