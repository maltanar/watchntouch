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
}

void DrawingData::saveSVG(QString fileName)
{
    QSvgGenerator svgGen;
    svgGen.setFileName(fileName);
    svgGen.setSize(sceneRect().size().toSize());
    svgGen.setTitle("Watch and Touch Drawing");
    svgGen.setDescription(QDateTime::currentDateTime().toString());

    QPainter svgPainter;
    svgPainter.begin(&svgGen);
    render(&svgPainter);
    svgPainter.end();
}

void DrawingData::loadSVG(QString fileName)
{
    // TODO QGraphicsSvgItem is rendered as a bitmap on the view...we'd prefer to have it vectoral
    QGraphicsSvgItem *svgContent = new QGraphicsSvgItem(fileName);
    clear();
    undoStack.clear();
    delete currentAction;
    currentAction =  new DrawingAction(this);
    addItem(svgContent);
}

QUndoStack * DrawingData::getUndoStack()
{
    return &undoStack;
}

void DrawingData::addItem(QGraphicsItem * item)
{
    currentAction->addDrawingStep(item);
    QGraphicsScene::addItem(item);
}

void DrawingData::registerAction()
{
    undoStack.push(currentAction);
    currentAction =  new DrawingAction(this);
}
