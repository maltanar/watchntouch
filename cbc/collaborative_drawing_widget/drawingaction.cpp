/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

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
    p.setCompositionMode(QPainter::CompositionMode_Source);
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
