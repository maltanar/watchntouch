#include "drawingaction.h"

DrawingAction::DrawingAction(QGraphicsScene *scene, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    parentScene = scene;
}

void DrawingAction::undo()
{
    for(int i = 0; i < steps.count(); i++) {
        parentScene->removeItem(steps.at(i));
    }
}

void DrawingAction::redo()
{
    // TODO this function is executed when the item is pushed on the undo stack, and we add the QGraphicsItem's already before
    // the push, resulting in two adds (the second fails and doesn't do anywthing but this consumes some CPU time)
    // implement the QUndoCommand ID scene for grouping steps inside the command
    if(steps.count() > 0)
        if(steps.at(0)->scene() != 0)
            return;

    for(int i = 0; i < steps.count(); i++) {
        parentScene->addItem(steps.at(i));
    }
}

void DrawingAction::addDrawingStep(QGraphicsItem *newItem)
{
    steps.append(newItem);
}
