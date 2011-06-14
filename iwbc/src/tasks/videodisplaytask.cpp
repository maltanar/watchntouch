/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "videodisplaytask.h"
#include <QTime>

VideoDisplayTask::VideoDisplayTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
    m_videoCanvas = new QWidget(this);

    m_videoPlayer = new VideoUnderlay(m_videoCanvas);
    m_videoDraw = new AnnotationWidget(m_videoCanvas);

    m_panel = NULL;

    QStackedLayout *layoutForCanvas = new QStackedLayout();
    layoutForCanvas->addWidget(m_videoPlayer);
    layoutForCanvas->addWidget(m_videoDraw);
    layoutForCanvas->setStackingMode(QStackedLayout::StackAll);
    layoutForCanvas->setAlignment(m_videoPlayer, Qt::AlignHCenter);
    layoutForCanvas->setAlignment(m_videoDraw, Qt::AlignHCenter);

    m_videoCanvas->setLayout(layoutForCanvas);

    QVBoxLayout *layoutForVideo = new QVBoxLayout();
    layoutForVideo->addWidget(m_videoCanvas);
    //layoutForVideo->addWidget(m_panel);

    this->setLayout(layoutForVideo);

    setContentDisplay(m_videoPlayer);
    setAnnotationWidget(m_videoDraw);
    setContextMenu(new ContextMenu(this));

    // connect signals and slots for video
    // TODO move requestReadOnlyAnnotation signal connection to AnnotationWidget
    connect(m_videoPlayer, SIGNAL(requestReadOnlyAnnotation(bool)), m_videoDraw, SLOT(requestReadOnlyStatus(bool)));
    connect(m_videoPlayer, SIGNAL(mediaLengthUpdate(float)), this, SLOT(mediaLengthUpdate(float)));
    connect(m_videoPlayer, SIGNAL(timelineUpdate(float)), this, SLOT(timelineUpdate(float)));

    /*
        connect(m_panel, SIGNAL(timelineChanged(float)), m_videoPlayer, SLOT(seekTo(float)));
    */

    m_annotationWidget->raise();

    m_currentMediaLen = m_currentPos = 0;
}

int VideoDisplayTask::getTaskType()
{
    return TASK_VIDEO;
}
void VideoDisplayTask::activate()
{
    ContentDisplayTask::activate();

    // establish signal/slot connections with QML menu

    connect(m_panel, SIGNAL(playPause(bool)), this, SLOT(playPause(bool)));
    connect(m_panel, SIGNAL(gotoPrevAnnotation()), this, SLOT(gotoPrevAnnotation()));
    connect(m_panel, SIGNAL(gotoNextAnnotation()), this, SLOT(gotoNextAnnotation()));
    connect(m_panel, SIGNAL(sliderMouseDown(int,int)), this, SLOT(sliderMouseDown(int,int)));
    connect(m_panel, SIGNAL(sliderMouseUp(int,int)), this, SLOT(sliderMouseUp(int,int)));
    connect(m_panel, SIGNAL(sliderMouseMove(int,int)), this, SLOT(sliderMouseMove(int,int)));
    connect(m_panel, SIGNAL(volumeMouseDown(int)),this, SLOT(volumeMouseDown(int)));
    connect(m_panel, SIGNAL(volumeMouseMove(int)),this, SLOT(volumeMouseMove(int)));
    connect(m_panel, SIGNAL(volumeMouseUp(int)),this, SLOT(volumeMouseUp(int)));

    // TODO update panel information - current volume level, timeline pos
    mediaLengthUpdate(m_currentMediaLen);
    timelineUpdate(m_currentPos);
}

void VideoDisplayTask::deactivate()
{
    ContentDisplayTask::deactivate();

    // destroy signal/slot connections with QML menu

    disconnect(m_panel, SIGNAL(playPause(bool)), this, SLOT(playPause(bool)));
    disconnect(m_panel, SIGNAL(gotoPrevAnnotation()), this, SLOT(gotoPrevAnnotation()));
    disconnect(m_panel, SIGNAL(gotoNextAnnotation()), this, SLOT(gotoNextAnnotation()));
    disconnect(m_panel, SIGNAL(sliderMouseDown(int,int)), this, SLOT(sliderMouseDown(int,int)));
    disconnect(m_panel, SIGNAL(sliderMouseUp(int,int)), this, SLOT(sliderMouseUp(int,int)));
    disconnect(m_panel, SIGNAL(sliderMouseMove(int,int)), this, SLOT(sliderMouseMove(int,int)));
    disconnect(m_panel, SIGNAL(volumeMouseDown(int)),this, SLOT(volumeMouseDown(int)));
    disconnect(m_panel, SIGNAL(volumeMouseMove(int)),this, SLOT(volumeMouseMove(int)));
    disconnect(m_panel, SIGNAL(volumeMouseUp(int)),this, SLOT(volumeMouseUp(int)));
}

// QML incoming connection slots (receive data/notifications from QML)

void VideoDisplayTask::playPause(bool isPlaying)
{
    if(isPlaying)
        m_videoPlayer->play();
    else
        m_videoPlayer->pause();
}

void VideoDisplayTask::gotoPrevAnnotation()
{
    // TODO IMPORTANT implement this!
}

void VideoDisplayTask::gotoNextAnnotation()
{
    // TODO IMPORTANT implement this!
}

void VideoDisplayTask::sliderMouseDown(int timelineSizeInPixels, int relativePositionOfTimeIndicatorInPixels )
{

}

void VideoDisplayTask::sliderMouseUp( int timelineSizeInPixels, int relativePositionOfTimeIndicatorInPixels )
{

}

void VideoDisplayTask::sliderMouseMove( int timelineSizeInPixels, int relativePositionOfTimeIndicatorInPixels )
{
    float frac = ((float) relativePositionOfTimeIndicatorInPixels) / ((float) timelineSizeInPixels);
    m_videoPlayer->seekTo(m_currentMediaLen * frac);
}

void VideoDisplayTask::volumeMouseDown(int volume)
{
    qWarning() << "volume mdown" << volume;
    // TODO IMPORTANT implement volume control functionality on VideoUnderlay side
}

void VideoDisplayTask::volumeMouseUp(int volume)
{
    qWarning() << "volume mup" << volume;
}

void VideoDisplayTask::volumeMouseMove(int volume)
{
    qWarning() << "mmovemdown" << volume;
}

// QML outgoing (pass data/notification to QML)

void VideoDisplayTask::setSliderPosition(float position)
{
    qWarning() << "set slider pos" << position;
    QVariant pos= QVariant::fromValue(position);
    QMetaObject::invokeMethod(m_panel, "setSliderPosition", Q_ARG(QVariant, pos));
}

void VideoDisplayTask::setVideoTime(QString elapsed, QString remaining)
{
    QVariant el = QVariant::fromValue(elapsed), rm = QVariant::fromValue(remaining);
    QMetaObject::invokeMethod(m_panel, "setVideoTime", Q_ARG(QVariant, el), Q_ARG(QVariant, rm));
}

void VideoDisplayTask::setVolume(unsigned int volume)
{

}

// VideoPlayer incoming data

void VideoDisplayTask::mediaLengthUpdate(float newLength)
{
    qWarning() << "got new media length" << newLength;

    m_currentMediaLen = newLength;
}

void VideoDisplayTask::timelineUpdate(float newPos)
{
    m_currentPos = newPos;
    QTime remaining(0,0,0), elapsed(0,0,0);
    elapsed = elapsed.addMSecs(newPos);
    remaining = remaining.addMSecs(m_currentMediaLen - newPos);

    // TODO NOTSOIMPORTANT also handle hours, if any
    setVideoTime(elapsed.toString("mm:ss") , remaining.toString("mm:ss"));

    // TODO IMPORTANT move the timeline slider
    setSliderPosition(newPos / m_currentMediaLen);
}


void VideoDisplayTask::showHidePanel(bool show)
{
    QVariant showHide = QVariant::fromValue(show);

    QMetaObject::invokeMethod(m_panel, "showHideMultimedia", Q_ARG(QVariant, showHide));
}
