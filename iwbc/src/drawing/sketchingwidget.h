/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef SKETCHINGWIDGET_H
#define SKETCHINGWIDGET_H

#include "basedrawingwidget.h"
#include "contentmatcher.h"

class SketchingWidget : public BaseDrawingWidget
{
    Q_OBJECT
public:
    explicit SketchingWidget(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QString currentContent;
    QString currentContext;
    ContentMatcher matcher;

    QString getCurrentAnnotation();

signals:

public slots:
    void contentChanged(QString newContent);
    void contextChanged(QString newContext);
};

#endif // SKETCHINGWIDGET_H
