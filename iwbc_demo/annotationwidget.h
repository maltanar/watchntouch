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

private:
    QString currentContent;
    QString currentContext;
    QSize currentSize;
    ContentDisplay * currentContentDisplay;
    ContentMatcher matcher;

    QString getCurrentAnnotation();

signals:

public slots:
    void contentChanged(QString newContent);
    void contextChanged(QString newContext);

};

#endif // ANNOTATIONWIDGET_H
