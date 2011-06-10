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

    connect(m_contentDisplay, SIGNAL(pageNumberChanged(int,int)), this, SLOT(pageNumberChanged(int,int)));

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

    connect(m_panel, SIGNAL(goToFirstPage()), m_contentDisplay, SLOT(gotoFirstSlide()));
    connect(m_panel, SIGNAL(goToLastPage()), m_contentDisplay, SLOT(gotoLastSlide()));
    connect(m_panel, SIGNAL(goToNextPage()), m_contentDisplay, SLOT(gotoNextSlide()));
    connect(m_panel, SIGNAL(goToPrevPage()), m_contentDisplay, SLOT(gotoPrevSlide()));

}

void PresentationDisplayTask::deactivate()
{
    ContentDisplayTask::deactivate();

    // TODO disconnect qml menu signals

    disconnect(m_panel, SIGNAL(goToFirstPage()), m_contentDisplay, SLOT(gotoFirstSlide()));
    disconnect(m_panel, SIGNAL(goToLastPage()), m_contentDisplay, SLOT(gotoLastSlide()));
    disconnect(m_panel, SIGNAL(goToNextPage()), m_contentDisplay, SLOT(gotoNextSlide()));
    disconnect(m_panel, SIGNAL(goToPrevPage()), m_contentDisplay, SLOT(gotoPrevSlide()));
}

void PresentationDisplayTask::goToPageNumber(QString no)
{
    // TODO go to specified page
}

void PresentationDisplayTask::setSlideNumberDisplay(QString text)
{
    QVariant txt = QVariant::fromValue(text);
    QMetaObject::invokeMethod(m_panel, "setSlideNumber", Q_ARG(QVariant, txt));
}

void PresentationDisplayTask::showHidePanel(bool show)
{
    QVariant showHide = QVariant::fromValue(show);

    QMetaObject::invokeMethod(m_panel, "showHidePresentation", Q_ARG(QVariant, showHide));
}

void PresentationDisplayTask::pageNumberChanged(int pageNo, int pageCount)
{
    qWarning() << "page change:" << pageNo << "of" << pageCount;
    setSlideNumberDisplay(QString::number(pageNo) + " / " + QString::number(pageCount));
}
