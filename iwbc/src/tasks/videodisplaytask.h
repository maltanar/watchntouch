#ifndef VIDEODISPLAYTASK_H
#define VIDEODISPLAYTASK_H

#include "contentdisplaytask.h"
#include <QStackedLayout>
#include <videounderlay.h>
#include <videocontrolpanel.h>
#include <annotationwidget.h>
#include <QGroupBox>
#include <contextmenu.h>

// TODO IMPORTANT get timeline length from QML or use normalized values
#define TIMELINE_LEN    854

class VideoDisplayTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit VideoDisplayTask(QWidget *parent = 0);

    ContentType getContentType();

    void activate();
    void deactivate();

protected:
    QWidget * m_videoCanvas;
    VideoUnderlay * m_videoPlayer;
    AnnotationWidget * m_videoDraw;
    float m_currentMediaLen;

    // QML outgoing connection slots (pass data/notification to QML)
    void setSliderPosition(float position);
    void setVideoTime(QString elapsed, QString remaining);
    void setVolume(unsigned int volume);

    void showHidePanel(bool show);

signals:

public slots:
    // QML incoming connection slots (receive data/notifications from QML)
    void playPause(bool isPlaying);

    void gotoPrevAnnotation();
    void gotoNextAnnotation();

    void sliderMouseDown(int timelineSizeInPixels, int relativePositionOfTimeIndicatorInPixels );
    void sliderMouseUp( int timelineSizeInPixels, int relativePositionOfTimeIndicatorInPixels );
    void sliderMouseMove( int timelineSizeInPixels, int relativePositionOfTimeIndicatorInPixels );

    void volumeMouseDown(int volume);
    void volumeMouseUp(int volume);
    void volumeMouseMove(int volume);

    // VideoPlayer incoming connection slots
    void mediaLengthUpdate(float newLength);
    void timelineUpdate(float newPos);

};

#endif // VIDEODISPLAYTASK_H
