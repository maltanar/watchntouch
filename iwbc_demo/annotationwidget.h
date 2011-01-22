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

    // TODO we can't monitor window closing on widget level, do it on window level instead

signals:

public slots:
    void contentChanged(QString newContent);
    void contextChanged(QString newContext);

};

#endif // ANNOTATIONWIDGET_H
