#include "webpagedisplaytask.h"
#include "bookmarklist.h"
#include "eventgenerator.h"

WebPageDisplayTask::WebPageDisplayTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
    m_webCanvas = new QWidget(this);

    m_webDisplay = new WebpageDisplayWidget(m_webCanvas);
    m_webDraw = new AnnotationWidget(m_webCanvas);

    QStackedLayout *layoutForWebCanvas = new QStackedLayout();
    layoutForWebCanvas->addWidget(m_webDisplay);
    layoutForWebCanvas->addWidget(m_webDraw);
    layoutForWebCanvas->setStackingMode(QStackedLayout::StackAll);
    layoutForWebCanvas->setAlignment(m_webDisplay, Qt::AlignHCenter);
    layoutForWebCanvas->setAlignment(m_webDraw, Qt::AlignHCenter);

    m_webCanvas->setLayout(layoutForWebCanvas);

    QVBoxLayout *layoutForWeb = new QVBoxLayout();
    layoutForWeb->addWidget(m_webCanvas);

    this->setLayout(layoutForWeb);

    setContentDisplay(m_webDisplay);
    setAnnotationWidget(m_webDraw);
    setContextMenu(new ContextMenu(this));

    connect(m_webDisplay, SIGNAL(requestReadOnlyAnnotation(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));
    connect(m_webDisplay, SIGNAL(webPageUrlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
    connect(m_webDisplay, SIGNAL(webPageLoadProgress(int)), this, SLOT(loadProgress(int)));

    m_annotationWidget->raise();
}

int WebPageDisplayTask::getTaskType()
{
    return TASK_WEBPAGE;
}

void WebPageDisplayTask::urlChanged(QUrl newUrl)
{
    setWebGuiURLText(newUrl.toString());
}

ContentType WebPageDisplayTask::getContentType()
{
    return CONTENTTYPE_WEBPAGE;
}

void WebPageDisplayTask::showHidePanel(bool show)
{
    QVariant showHide = QVariant::fromValue(show);

    QMetaObject::invokeMethod(m_panel, "showHideWeb", Q_ARG(QVariant, showHide));
}


void WebPageDisplayTask::activate()
{
    ContentDisplayTask::activate();

    // connect swipe gesture
    connect(eventGenerator, SIGNAL(swipeGesture(int,int,int)), this, SLOT(swipeGesture(int,int,int)));

    // TODO connect QML slots and signals
    connect(m_panel, SIGNAL(back()), m_webDisplay, SLOT(back()));
    connect(m_panel, SIGNAL(next()), m_webDisplay, SLOT(forward()));
    connect(m_panel, SIGNAL(refresh()), m_webDisplay, SLOT(reload()));
    connect(m_panel, SIGNAL(webAnnotationStatus(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));
    connect(m_panel, SIGNAL(gotoURL(QString)), m_webDisplay, SLOT(loadWebPage(QString)));
    connect(m_panel, SIGNAL(gotoBookmark(QString)), m_webDisplay, SLOT(loadWebPage(QString)));
    connect(m_panel, SIGNAL(bookmarkRequest(QString)), this, SLOT(bookmarkRequest(QString)));
    connect(m_panel, SIGNAL(deleteBookmarkRequest(QString,int)), this, SLOT(deleteBookmarkRequest(QString,int)));

    /*signal back()
    signal next()
    signal refresh()*/

    // sync with webpage status
    // TODO sync other stats as well
    setWebGuiReadOnlyStatus(m_annotationWidget->isReadOnly());
    setWebGuiURLText(m_webDisplay->getCurrentURL().toString());
    updateBookmarksList();
}

void WebPageDisplayTask::deactivate()
{
    ContentDisplayTask::deactivate();

    // disconnect swipe gesture
    disconnect(eventGenerator, SIGNAL(swipeGesture(int,int,int)), this, SLOT(swipeGesture(int,int,int)));

    // TODO disconnect QML slots and signals
    disconnect(m_panel, SIGNAL(back()), m_webDisplay, SLOT(back()));
    disconnect(m_panel, SIGNAL(next()), m_webDisplay, SLOT(forward()));
    disconnect(m_panel, SIGNAL(refresh()), m_webDisplay, SLOT(reload()));
    disconnect(m_panel, SIGNAL(webAnnotationStatus(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));
    disconnect(m_panel, SIGNAL(gotoURL(QString)), m_webDisplay, SLOT(loadWebPage(QString)));
    disconnect(m_panel, SIGNAL(gotoBookmark(QString)), m_webDisplay, SLOT(loadWebPage(QString)));
    disconnect(m_panel, SIGNAL(bookmarkRequest(QString)), this, SLOT(bookmarkRequest(QString)));
    disconnect(m_panel, SIGNAL(deleteBookmarkRequest(QString,int)), this, SLOT(deleteBookmarkRequest(QString,int)));
    disconnect(m_webDisplay, SIGNAL(webPageLoadProgress(int)), m_panel, SLOT(webLoadingBar(int)));

    // save the bookmarks list at this point
    bookmarkList->writeToStorage();
}

void WebPageDisplayTask::setWebGuiReadOnlyStatus(bool readOnly)
{
    QVariant bReadOnly = QVariant::fromValue(readOnly);

    QMetaObject::invokeMethod(m_panel, "setWebReadOnlyStatus", Q_ARG(QVariant, bReadOnly));
}

void WebPageDisplayTask::setWebGuiURLText(QString urlString)
{
    QVariant sUrlString = QVariant::fromValue(urlString);

    QMetaObject::invokeMethod(m_panel, "setURLText", Q_ARG(QVariant, sUrlString));
}

void WebPageDisplayTask::webGuiAddBookmark(QString urlString)
{
    QVariant sUrlString = QVariant::fromValue(urlString);

    QMetaObject::invokeMethod(m_panel, "addBookmark", Q_ARG(QVariant, sUrlString));
}

void WebPageDisplayTask::webGuiRemoveBookmark(int index)
{
    QVariant iIndex = QVariant::fromValue(index);

    QMetaObject::invokeMethod(m_panel, "deleteBookmark", Q_ARG(QVariant, iIndex));
}

void WebPageDisplayTask::bookmarkRequest(QString URL)
{
    qWarning() << "got bookmark request for" << URL;

    if(!bookmarkList->contains(URL)) {
        webGuiAddBookmark(URL);
        bookmarkList->append(URL);  // TODO check for duplicates before adding!
    }

    // TODO IMPORTANT refresh bookmarks when button pressed? notify with signals?
}

void WebPageDisplayTask::deleteBookmarkRequest(QString URL, int index)
{
    qWarning() << "remove bookmark" << URL << "index" << index;

    if(bookmarkList->contains(URL))  {
        bookmarkList->removeOne(URL);
        webGuiRemoveBookmark(index);
    }

    // TODO IMPORTANT remove given bookmark from storage
}

void WebPageDisplayTask::updateBookmarksList()
{
    webGuiClearBookmarks();

    for(int i=0; i<bookmarkList->count(); i++)
        webGuiAddBookmark(bookmarkList->at(i));
}


void WebPageDisplayTask::webGuiClearBookmarks()
{
    QMetaObject::invokeMethod(m_panel, "clearBookmarkList");
}

void WebPageDisplayTask::loadProgress(int progress)
{
    QVariant iProgress = QVariant::fromValue(progress);

    QMetaObject::invokeMethod(m_panel, "webLoadingBar", Q_ARG(QVariant, iProgress));
}

void WebPageDisplayTask::swipeGesture(int direction, int dx, int dy)
{
    qWarning() << this << "got swipeGesture with direction" << direction << "and deltas" << dx << dy;

    if(direction == GESTURE_DIR_DOWN) {
        m_webDisplay->scrollWebPage(dx, dy);
    }
}
