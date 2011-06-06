#include "videodisplaytask.h"

VideoDisplayTask::VideoDisplayTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
    m_videoCanvas = new QWidget(this);

    m_videoPlayer = new VideoUnderlay(m_videoCanvas);
    m_videoDraw = new AnnotationWidget(m_videoCanvas);

    m_videoPanel = new VideoControlPanel(this);

    QStackedLayout *layoutForCanvas = new QStackedLayout();
    layoutForCanvas->addWidget(m_videoPlayer);
    layoutForCanvas->addWidget(m_videoDraw);
    layoutForCanvas->setStackingMode(QStackedLayout::StackAll);
    layoutForCanvas->setAlignment(m_videoPlayer, Qt::AlignHCenter);
    layoutForCanvas->setAlignment(m_videoDraw, Qt::AlignHCenter);

    m_videoCanvas->setLayout(layoutForCanvas);

    QVBoxLayout *layoutForVideo = new QVBoxLayout();
    layoutForVideo->addWidget(m_videoCanvas);
    layoutForVideo->addWidget(m_videoPanel);

    this->setLayout(layoutForVideo);

    setContentDisplay(m_videoPlayer);
    setAnnotationWidget(m_videoDraw);
    setContextMenu(new ContextMenu(this));

    // connect signals and slots for video
    // TODO move requestReadOnlyAnnotation signal connection to AnnotationWidget
    // TODO connect video signals and slots with QML
    connect(m_videoPanel, SIGNAL(playClicked()), m_videoPlayer, SLOT(play()));
    connect(m_videoPlayer, SIGNAL(requestReadOnlyAnnotation(bool)), m_videoDraw, SLOT(requestReadOnlyStatus(bool)));
    connect(m_videoPanel, SIGNAL(pauseClicked()), m_videoPlayer, SLOT(pause()));
    connect(m_videoPanel, SIGNAL(timelineChanged(float)), m_videoPlayer, SLOT(seekTo(float)));
    connect(m_videoPlayer, SIGNAL(mediaLengthUpdate(float)), m_videoPanel, SLOT(mediaLengthChanged(float)));
    connect(m_videoPlayer, SIGNAL(timelineUpdate(float)), m_videoPanel, SLOT(mediaPosChanged(float)));

    m_annotationWidget->raise();
}

ContentType VideoDisplayTask::getContentType()
{
    return CONTENTTYPE_VIDEO;
}
