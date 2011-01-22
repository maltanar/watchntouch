#ifndef DRAWINGDATA_H
#define DRAWINGDATA_H

#include <QGraphicsScene>
#include <QSvgGenerator>
#include <QList>
#include <QUndoStack>

#include "drawingaction.h"

class DrawingData : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DrawingData(QObject *parent = 0);
    void saveSVG(QString fileName);
    void loadSVG(QString fileName);

    QUndoStack * getUndoStack();
    void addItem(QGraphicsItem * item);
    void registerAction();
    void clear();

    bool isModified();
    void setModified(bool newValue);

private:
    QUndoStack undoStack;
    DrawingAction *currentAction;
    bool modified;

signals:

public slots:

};

#endif // DRAWINGDATA_H
