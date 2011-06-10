#include "presentationdisplaytask.h"

PresentationDisplayTask::PresentationDisplayTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
    setContentDisplay(new PresentationDisplayWidget(this));
    setAnnotationWidget(new AnnotationWidget(this));
    setContextMenu(new ContextMenu(this));

    m_layout.addWidget(m_contentDisplay);
    m_layout.addWidget(m_annotationWidget);

    m_layout.setStackingMode(QStackedLayout::StackAll);
    m_layout.setAlignment(m_contentDisplay, Qt::AlignHCenter);
    m_layout.setAlignment(m_annotationWidget, Qt::AlignHCenter);

    setLayout(&m_layout);

    m_annotationWidget->raise();
}

ContentType PresentationDisplayTask::getContentType()
{
    return CONTENTTYPE_PRESENTATION;
}

void PresentationDisplayTask::activate()
{
    ContentDisplayTask::activate();

    // TODO connect qml menu signals

}

void PresentationDisplayTask::deactivate()
{
    ContentDisplayTask::deactivate();

    // TODO disconnect qml menu signals
}

void PresentationDisplayTask::showHidePanel(bool show)
{
    QVariant showHide = QVariant::fromValue(show);

    QMetaObject::invokeMethod(m_panel, "showHidePresentation", Q_ARG(QVariant, showHide));
}
