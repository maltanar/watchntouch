/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

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
