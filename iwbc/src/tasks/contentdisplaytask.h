/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

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

    virtual void setPanel(QObject *panel);

    virtual void activate();
    virtual void deactivate();

    virtual int getTaskType();

    QImage getTaskScreenshot(int maxHeight = 120);

protected:
    ContextMenu * m_contextMenu;
    AnnotationWidget * m_annotationWidget;
    ContentDisplay * m_contentDisplay;
    QObject * m_panel;

    virtual void setAnnotationWidget(AnnotationWidget * newWidget);
    virtual void setContentDisplay(ContentDisplay * newDisplay);
    virtual void setContextMenu(ContextMenu *newMenu);

    virtual void showHidePanel(bool show) = 0;
    virtual int getPanelHeight();

    void resizeEvent(QResizeEvent *);

signals:

public slots:
    void mousePress(QPoint p, int button, int buttons);
    void mouseMove(QPoint p, int button, int buttons);
    void mouseRelease(QPoint p, int button, int buttons);
    void print();

protected slots:
    void showContextMenu(QPoint p);
    void pinchGesture(QPoint center, bool inOut);

};

#endif // CONTENTDISPLAYTASK_H

