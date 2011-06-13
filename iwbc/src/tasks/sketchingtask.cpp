#include "sketchingtask.h"

SketchingTask::SketchingTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
    m_contextMenu = new ContextMenu(this);
    m_sketchingWidget = new SketchingWidget(this);

    m_layout.addWidget(m_sketchingWidget);

    setLayout(&m_layout);

    m_contextMenu->hide();

    connect(m_sketchingWidget, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

    // connect the context menu signals/slots with the sketching widget
    connect(m_contextMenu,SIGNAL(colorSelected(QColor)),m_sketchingWidget, SLOT(setDrawingColor(QColor)));
    connect(m_contextMenu, SIGNAL(undo()), m_sketchingWidget->getDrawingData()->getUndoStack(), SLOT(undo()));
    connect(m_contextMenu, SIGNAL(redo()), m_sketchingWidget->getDrawingData()->getUndoStack(), SLOT(redo()));
    connect(m_contextMenu, SIGNAL(toolSelected(DrawingMode)), m_sketchingWidget, SLOT(setDrawingMode(DrawingMode)));
    connect(m_contextMenu, SIGNAL(penWidthIncrease()), m_sketchingWidget, SLOT(increasePenWidth()));
    connect(m_contextMenu, SIGNAL(penWidthDecrease()), m_sketchingWidget, SLOT(decreasePenWidth()));
    connect(m_contextMenu, SIGNAL(eraserSize(int)), m_sketchingWidget, SLOT(setEraserSize(int)));
    connect(m_contextMenu, SIGNAL(erasePage()), m_sketchingWidget, SLOT(clear()));
}

int SketchingTask::getTaskType()
{
    return TASK_SKETCHING;
}

void SketchingTask::showHidePanel(bool show) {
    // sketching has no panel so we don't need to do anything here
}

void SketchingTask::newSketch(QString id)
{
    m_sketchingWidget->contextChanged(id);
}

void SketchingTask::sketchFromImage(QString id, QPixmap img)
{
    img.save(m_matcher.matchingAnnotation("sketch", id), "PNG");
    m_sketchingWidget->contextChanged(id);
}
