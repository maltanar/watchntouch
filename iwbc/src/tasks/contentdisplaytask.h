#ifndef CONTENTDISPLAYTASK_H
#define CONTENTDISPLAYTASK_H

#include <QWidget>
#include <QStackedLayout>
#include <contentdisplay.h>
#include <annotationwidget.h>
#include <contextmenu.h>

class ContentDisplayTask : public QWidget
{
    Q_OBJECT
public:
    explicit ContentDisplayTask(QWidget *parent = 0);

    ContentDisplay * getContentDisplay();
    AnnotationWidget * getAnnotationWidget();
    ContextMenu * getContextMenu();

protected:
    ContextMenu * m_contextMenu;
    AnnotationWidget * m_annotationWidget;
    ContentDisplay * m_contentDisplay;

    virtual void setAnnotationWidget(AnnotationWidget * newWidget);
    virtual void setContentDisplay(ContentDisplay * newDisplay);
    virtual void setContextMenu(ContextMenu *newMenu);

    void resizeEvent(QResizeEvent *);

signals:

public slots:

private slots:
    void showContextMenu(QPoint p);

};

#endif // CONTENTDISPLAYTASK_H
