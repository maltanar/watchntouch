#ifndef DRAWINGACTION_H
#define DRAWINGACTION_H

#include <QUndoCommand>
#include <QPixmap>
#include <QPicture>
#include <QRect>

#include "drawingdata.h"

class DrawingData;

class DrawingAction : public QUndoCommand
{
public:
    explicit DrawingAction(DrawingData *scene, QUndoCommand *parent = 0);
    void setActions(QPicture actions);
    void setPrevPixmap(QPixmap prev, QRect rect);
    void undo();
    void redo();

private:
    DrawingData * parentScene;
    QPicture drawingActions;
    QPixmap prevPixmap;
    QRect undoRect;

signals:

public slots:

};

#endif // DRAWINGACTION_H
