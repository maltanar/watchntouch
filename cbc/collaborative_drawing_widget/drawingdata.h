/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef DRAWINGDATA_H
#define DRAWINGDATA_H

#include <QGraphicsScene>
#include <QList>
#include <QUndoStack>
#include <QPixmap>
#include <QPicture>

#include "drawingaction.h"

class DrawingAction;

class DrawingData : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DrawingData(QObject *parent = 0);
    void saveImage(QString fileName);
    void loadImage(QString fileName);

    QUndoStack * getUndoStack();
    void registerAction(QPicture actions);
    void clear();

    bool isModified();
    void setModified(bool newValue);

    QPixmap * getStage();

protected:
    void drawBackground ( QPainter * painter, const QRectF & rect );

private:
    QUndoStack undoStack;
    DrawingAction *currentAction;
    bool modified;
    QPixmap *stage;

signals:

public slots:

};

#endif // DRAWINGDATA_H
