#ifndef SKETCHINGTASK_H
#define SKETCHINGTASK_H

#include "contentdisplaytask.h"
#include "sketchingwidget.h"
#include "contextmenu.h"
#include <QVBoxLayout>

class SketchingTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit SketchingTask(QWidget *parent = 0);
    ContentType getContentType();

    void newSketch(QString id);

protected:
    void showHidePanel(bool show);

    QVBoxLayout m_layout;
    SketchingWidget *m_sketchingWidget;

signals:

public slots:

};

#endif // SKETCHINGTASK_H
