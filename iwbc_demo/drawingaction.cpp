#include "drawingaction.h"

#include <QPainter>

DrawingAction::DrawingAction(DrawingData *scene, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    parentScene = scene;
}

void DrawingAction::undo()
{
    QPainter p(parentScene->getStage());
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.drawPixmap(undoRect.topLeft(),prevPixmap);
    p.end();
    parentScene->update();
}

void DrawingAction::redo()
{
    QPainter p(parentScene->getStage());
    drawingActions.play(&p);
    p.end();
    parentScene->update();
}

void DrawingAction::setActions(QPicture actions)
{
    // manually copy data, otherwise all redo steps end up being the same
    drawingActions.setData(actions.data(), actions.size());
}

void DrawingAction::setPrevPixmap(QPixmap prev, QRect rect)
{
    prevPixmap = prev;
    undoRect = rect;
}
