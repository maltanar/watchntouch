#include <QUrl>
#include "presentationdisplaytask.h"
#include "eventgenerator.h"

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
    connect(m_contentDisplay, SIGNAL(pageThumbnailReady(QString, int)), this, SLOT(pageThumbReady(QString,int)));

    m_annotationWidget->raise();

    m_currentPage = 0;
    m_pageCount = 0;
}

int PresentationDisplayTask::getTaskType()
{
    return TASK_PRESENTATION;
}

ContentType PresentationDisplayTask::getContentType()
{
    return CONTENTTYPE_PRESENTATION;
}

void PresentationDisplayTask::activate()
{
    ContentDisplayTask::activate();

    // connect swipe gesture
    connect(eventGenerator, SIGNAL(swipeGesture(int,int,int)), this, SLOT(swipeGesture(int,int,int)));

    connect(m_panel, SIGNAL(goToFirstPage()), m_contentDisplay, SLOT(gotoFirstSlide()));
    connect(m_panel, SIGNAL(goToLastPage()), m_contentDisplay, SLOT(gotoLastSlide()));
    connect(m_panel, SIGNAL(goToNextPage()), m_contentDisplay, SLOT(gotoNextSlide()));
    connect(m_panel, SIGNAL(goToPrevPage()), m_contentDisplay, SLOT(gotoPrevSlide()));
    connect(m_panel, SIGNAL(goToPageNumber(QString)), this, SLOT(goToPageNumber(QString)));

    // TODO IMPORTANT clear and insert the thumbnails, align to current page
    updateThumbnails();
    setSlideNumberDisplay(QString::number(m_currentPage) + " / " + QString::number(m_pageCount));
    PresentationGui_alignPageScrollerToPageNumber(m_currentPage);
}

void PresentationDisplayTask::deactivate()
{
    ContentDisplayTask::deactivate();

    // disconnect swipe gesture
    disconnect(eventGenerator, SIGNAL(swipeGesture(int,int,int)), this, SLOT(swipeGesture(int,int,int)));

    disconnect(m_panel, SIGNAL(goToFirstPage()), m_contentDisplay, SLOT(gotoFirstSlide()));
    disconnect(m_panel, SIGNAL(goToLastPage()), m_contentDisplay, SLOT(gotoLastSlide()));
    disconnect(m_panel, SIGNAL(goToNextPage()), m_contentDisplay, SLOT(gotoNextSlide()));
    disconnect(m_panel, SIGNAL(goToPrevPage()), m_contentDisplay, SLOT(gotoPrevSlide()));
    disconnect(m_panel, SIGNAL(goToPageNumber(QString)), this, SLOT(goToPageNumber(QString)));
}

void PresentationDisplayTask::goToPageNumber(QString no)
{
    // TODO go to specified page
    qWarning() << "go to page number" << no;
    ((PresentationDisplayWidget*)m_contentDisplay)->gotoSlide(no.toInt());
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
    m_currentPage = pageNo;
    m_pageCount = pageCount;
    qWarning() << "page change:" << pageNo << "of" << pageCount;
    setSlideNumberDisplay(QString::number(pageNo) + " / " + QString::number(pageCount));
    PresentationGui_alignPageScrollerToPageNumber(pageNo);
}

int PresentationDisplayTask::getPanelHeight()
{
    QVariant ret_arg = QVariant::fromValue(0);
    QMetaObject::invokeMethod(m_panel, "getPresentationPanelHeight", Q_RETURN_ARG(QVariant, ret_arg));

    return ret_arg.toInt();
}

void PresentationDisplayTask::PresentationGui_addToPageScroller(QString path, int pageNo)
{
    qWarning() << "PresentationGui_addToPageScroller" << path << pageNo;
    QVariant sImagePath = QVariant::fromValue("file://"+path);
    QVariant iPageNo = QVariant::fromValue(pageNo);
    QMetaObject::invokeMethod(m_panel, "addToPageScroller", Q_ARG(QVariant, sImagePath), Q_ARG(QVariant, iPageNo));
}

void PresentationDisplayTask::PresentationGui_clearPageScroller()
{
    qWarning() << "PresentationGui_clearPageScroller";
    QMetaObject::invokeMethod(m_panel, "clearPageScroller");
}

void PresentationDisplayTask::PresentationGui_alignPageScrollerToPageNumber(int pageNo)
{
    QVariant iPageNo = QVariant::fromValue(pageNo);
    QMetaObject::invokeMethod(m_panel, "alignPageScrollerToPageNumber", Q_ARG(QVariant, iPageNo));
}

void PresentationDisplayTask::PresentationGui_setPageScrollerImageAtIndex(QString pathOfTheImage, int pageNo)
{
    qWarning() << "PresentationGui_setPageScrollerImageAtIndex" << pathOfTheImage << pageNo;
    QVariant sImagePath = QVariant::fromValue(pathOfTheImage);
    QVariant iPageNo = QVariant::fromValue(QString::number(pageNo));
    QMetaObject::invokeMethod(m_panel, "setPageScrollerImageAtIndex", Q_ARG(QVariant, sImagePath), Q_ARG(QVariant, iPageNo));
}

void PresentationDisplayTask::pageThumbReady(QString path, int pageNo)
{
    m_thumbs.append(path);
    PresentationGui_addToPageScroller(path, pageNo);
}

void PresentationDisplayTask::updateThumbnails()
{
    // TODO IMPORTANT insert / update the thumbnails inside the list
    PresentationGui_clearPageScroller();

    for(int i = 0; i < m_thumbs.count(); i++) {
        PresentationGui_addToPageScroller(m_thumbs.at(i), i+1);
    }
}

void PresentationDisplayTask::swipeGesture(int direction, int dx, int dy)
{
    qWarning() << this << "got swipeGesture with direction" << direction << "and deltas" << dx << dy;

    if(direction == GESTURE_DIR_RIGHT) {
        ((PresentationDisplayWidget*)m_contentDisplay)->gotoPrevSlide();
    } else if(direction == GESTURE_DIR_LEFT) {
        ((PresentationDisplayWidget*)m_contentDisplay)->gotoNextSlide();
    }
}
