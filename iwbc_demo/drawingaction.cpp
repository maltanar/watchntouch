#include "drawingaction.h"

#include <QPainter>

DrawingAction::DrawingAction(DrawingData *scene, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    parentScene = scene;
}

void DrawingAction::undo()
{
    qWarning() << "undoing" << prevPixmap.size() << undoRect;
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
    drawingActions = actions;
}

void DrawingAction::setPrevPixmap(QPixmap prev, QRect rect)
{
    prevPixmap = prev;
    undoRect = rect;
}
