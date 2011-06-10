#ifndef CONTENTDISPLAYTASK_H
#define CONTENTDISPLAYTASK_H

#include <QWidget>
#include <QStackedLayout>
#include <QImage>
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
    virtual ContentType getContentType() = 0;

    virtual void setPanel(QObject *panel);

    virtual void activate();
    virtual void deactivate();

protected:
    ContextMenu * m_contextMenu;
    AnnotationWidget * m_annotationWidget;
    ContentDisplay * m_contentDisplay;
    QObject * m_panel;

    virtual void setAnnotationWidget(AnnotationWidget * newWidget);
    virtual void setContentDisplay(ContentDisplay * newDisplay);
    virtual void setContextMenu(ContextMenu *newMenu);

    virtual void showHidePanel(bool show) = 0;

    void resizeEvent(QResizeEvent *);

signals:

public slots:
    QImage getTaskScreenshot();
    void mousePress(QPoint p, int button, int buttons);
    void mouseMove(QPoint p, int button, int buttons);
    void mouseRelease(QPoint p, int button, int buttons);

protected slots:
    void showContextMenu(QPoint p);

};

#endif // CONTENTDISPLAYTASK_H

