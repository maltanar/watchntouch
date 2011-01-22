#ifndef DRAWINGACTION_H
#define DRAWINGACTION_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>

class DrawingAction : public QUndoCommand
{
public:
    explicit DrawingAction(QGraphicsScene *scene, QUndoCommand *parent = 0);
    void addDrawingStep(QGraphicsItem *newItem);
    void undo();
    void redo();

private:
    QGraphicsScene * parentScene;
    QList<QGraphicsItem *> steps;

signals:

public slots:

};

#endif // DRAWINGACTION_H
