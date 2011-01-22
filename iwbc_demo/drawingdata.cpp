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
    delete currentAction;
    currentAction =  new DrawingAction(this);
    addItem(svgContent);

    setModified(false);
}

QUndoStack * DrawingData::getUndoStack()
{
    return &undoStack;
}

void DrawingData::addItem(QGraphicsItem * item)
{
    currentAction->addDrawingStep(item);
    QGraphicsScene::addItem(item);
    setModified(true);
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
}
