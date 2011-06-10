#ifndef WEBPAGEDISPLAYTASK_H
#define WEBPAGEDISPLAYTASK_H

#include "contentdisplaytask.h"
#include <QStackedLayout>
#include <webpagedisplaywidget.h>
#include <webcontrolpanel.h>
#include <annotationwidget.h>
#include <QGroupBox>
#include <contextmenu.h>

class WebPageDisplayTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit WebPageDisplayTask(QWidget *parent = 0);
    ContentType getContentType();

protected:
    QWidget * m_webCanvas;
    WebpageDisplayWidget * m_webDisplay;
    AnnotationWidget * m_webDraw;
    WebControlPanel * m_webControlPanel;

    void showHidePanel(bool show);

signals:

public slots:

};

#endif // WEBPAGEDISPLAYTASK_H
