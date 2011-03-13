#include "drawingdata.h"

#include <QDateTime>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
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

void DrawingData::saveSVG(QString fileName)
{
    // TODO do not save anything if annotation is empty, maybe delete old file?
    QSvgGenerator svgGen;
    svgGen.setFileName(fileName);
    svgGen.setSize(sceneRect().size().toSize());
    svgGen.setTitle("Watch and Touch Drawing");
    svgGen.setDescription(QDateTime::currentDateTime().toString());

    QPainter svgPainter;
    svgPainter.begin(&svgGen);
    render(&svgPainter);
    svgPainter.end();

    setModified(false);
}

void DrawingData::loadSVG(QString fileName)
{
    // TODO QGraphicsSvgItem is rendered as a bitmap on the view...we'd prefer to have it vectoral
    QGraphicsSvgItem *svgContent = new QGraphicsSvgItem(fileName);
    clear();
    QGraphicsScene::addItem(svgContent);

    setModified(false);
}

QUndoStack * DrawingData::getUndoStack()
{
    return &undoStack;
}

void DrawingData::addItem(QGraphicsItem * item)
{
    //currentAction->addDrawingStep(item);
    QGraphicsScene::addItem(item);
    //setModified(true);
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
