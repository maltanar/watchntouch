#ifndef PRESENTATIONDISPLAYTASK_H
#define PRESENTATIONDISPLAYTASK_H

#include "contentdisplaytask.h"
#include <QStackedLayout>
#include <presentationdisplaywidget.h>
#include <annotationwidget.h>
#include <contextmenu.h>

class PresentationDisplayTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit PresentationDisplayTask(QWidget *parent = 0);

    ContentType getContentType();

    void activate();
    void deactivate();

protected:
    QStackedLayout m_layout;

    void showHidePanel(bool show);


signals:

public slots:

};

#endif // PRESENTATIONDISPLAYTASK_H
