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
    bool m_isReadOnly;
    QPointF sceneCenter;

    QString getCurrentAnnotation();

signals:

public slots:
    void contentChanged(QString newContent);
    void contextChanged(QString newContext);
    void scrollRequest(int dx, int dy);
    void requestReadOnlyStatus(bool readOnly);

};

#endif // ANNOTATIONWIDGET_H
