/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef QMLMENULAYER_H
#define QMLMENULAYER_H

#include <QDeclarativeView>

class QMLMenuLayer : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit QMLMenuLayer(QWidget *parent = 0);

protected:
    unsigned int m_qmlWidth, m_qmlHeight;

    void setQMLMask();
    void resizeEvent(QResizeEvent *event);

signals:
    void mousePressSignal(QPoint p, int button, int buttons);
    void mouseMoveSignal(QPoint p, int button, int buttons);
    void mouseReleaseSignal(QPoint p, int button, int buttons);

public slots:

protected slots:
    void qmlMousePressed(int x, int y, int button, int buttons);
    void qmlMouseMoved(int x, int y, int button, int buttons);
    void qmlMouseReleased(int x, int y, int button, int buttons);

};

#endif // QMLMENULAYER_H
