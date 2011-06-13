#ifndef COLLABORATIVEDRAWINGWIDGET_H
#define COLLABORATIVEDRAWINGWIDGET_H

#include "basedrawingwidget.h"
#include <QtNetwork>

class CollaborativeDrawingWidget : public BaseDrawingWidget
{
    Q_OBJECT
public:
    explicit CollaborativeDrawingWidget(QWidget *parent = 0);

protected:
    void commitDrawing(QPicture drawingData);

    QString m_currentSession;

signals:
    void drawingCommited(QString sessionName, QPicture drawingData);

public slots:
    void drawingArrived(QString sessionName, QByteArray picData, bool isInitialState);

};

#endif // COLLABORATIVEDRAWINGWIDGET_H
