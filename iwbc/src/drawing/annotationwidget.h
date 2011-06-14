/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef ANNOTATIONWIDGET_H
#define ANNOTATIONWIDGET_H

#include "basedrawingwidget.h"
#include "contentmatcher.h"
#include "contentdisplay.h"

class AnnotationWidget : public BaseDrawingWidget
{
    Q_OBJECT
public:
    explicit AnnotationWidget(QWidget *parent = 0);

    void attachToContentDisplay(ContentDisplay *display);

    bool isReadOnly();

private:
    QString currentContent;
    QString currentContext;
    QSize currentSize;
    ContentDisplay * currentContentDisplay;
    ContentMatcher matcher;
    bool m_isReadOnly;
    QPointF sceneCenter;

    QString getCurrentAnnotation();

signals:

public slots:
    void contentChanged(QString newContent);
    void contextChanged(QString newContext);
    void scrollRequest(int dx, int dy);
    void requestReadOnlyStatus(bool readOnly);
    void requestSave();
    void clearAll();

};

#endif // ANNOTATIONWIDGET_H
