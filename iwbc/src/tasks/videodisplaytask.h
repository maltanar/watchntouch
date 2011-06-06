#ifndef VIDEODISPLAYTASK_H
#define VIDEODISPLAYTASK_H

#include "contentdisplaytask.h"
#include <QStackedLayout>
#include <videounderlay.h>
#include <videocontrolpanel.h>
#include <annotationwidget.h>
#include <QGroupBox>
#include <contextmenu.h>

class VideoDisplayTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit VideoDisplayTask(QWidget *parent = 0);

    ContentType getContentType();

protected:
    QWidget * m_videoCanvas;
    VideoUnderlay * m_videoPlayer;
    AnnotationWidget * m_videoDraw;
    VideoControlPanel * m_videoPanel;

signals:

public slots:

};

#endif // VIDEODISPLAYTASK_H
