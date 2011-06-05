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

protected:
    QWidget * m_webScrollArea;

signals:

public slots:

};

#endif // WEBPAGEDISPLAYTASK_H
