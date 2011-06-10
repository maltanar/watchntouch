#ifndef QMLMENULAYER_H
#define QMLMENULAYER_H

#include <QDeclarativeView>

class QMLMenuLayer : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit QMLMenuLayer(QWidget *parent = 0);

protected:
    unsigned int m_maskHeight;

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
