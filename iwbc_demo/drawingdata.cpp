#include "drawingdata.h"

#include <QDateTime>
#include <QPainter>

#include <QDebug>

DrawingData::DrawingData(QObject *parent) :
    QGraphicsScene(parent)
{
    currentAction = new DrawingAction(this);
    modified = false;
    stage = new QPixmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    stage->fill(Qt::transparent);
}

void DrawingData::saveImage(QString fileName)
{
    // TODO do not save anything if annotation is empty, maybe delete old file?
    qWarning() << "saving to" << fileName;
    stage->save(fileName, "png");
    setModified(false);
}

void DrawingData::loadImage(QString fileName)
{
    clear();
    // TODO should check for the file's resolution and upscale/downscale to current if needed
    stage->load(fileName, "png");

    setModified(false);
}

QUndoStack * DrawingData::getUndoStack()
{
    return &undoStack;
}

void DrawingData::registerAction()
{
    undoStack.push(currentAction);
    currentAction =  new DrawingAction(this);
}

bool DrawingData::isModified()
{
    return modified;
}

void DrawingData::setModified(bool newValue)
{
    modified = newValue;
}

void DrawingData::clear()
{
    QGraphicsScene::clear();
    undoStack.clear();
    setModified(false);

    if(currentAction) delete currentAction;
    currentAction =  new DrawingAction(this);

    // clear the stage pixmap
    stage->fill(Qt::transparent);
}

QPixmap * DrawingData::getStage()
{
    return stage;
}

void DrawingData::drawBackground ( QPainter * painter, const QRectF & rect )
{
    // draw the stage pixmap as the scene background
    // the stage pixmap is where we do all the user-created drawing
    painter->setClipRect(rect);
    painter->drawPixmap(0,0, *stage);
}
