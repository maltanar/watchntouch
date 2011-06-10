#include "webpagedisplaytask.h"

WebPageDisplayTask::WebPageDisplayTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
    m_webCanvas = new QWidget(this);

    m_webDisplay = new WebpageDisplayWidget(m_webCanvas);
    m_webDraw = new AnnotationWidget(m_webCanvas);

    m_webControlPanel = new WebControlPanel(this);

    QStackedLayout *layoutForWebCanvas = new QStackedLayout();
    layoutForWebCanvas->addWidget(m_webDisplay);
    layoutForWebCanvas->addWidget(m_webDraw);
    layoutForWebCanvas->setStackingMode(QStackedLayout::StackAll);
    layoutForWebCanvas->setAlignment(m_webDisplay, Qt::AlignHCenter);
    layoutForWebCanvas->setAlignment(m_webDraw, Qt::AlignHCenter);

    m_webCanvas->setLayout(layoutForWebCanvas);

    QVBoxLayout *layoutForWeb = new QVBoxLayout();
    layoutForWeb->addWidget(m_webCanvas);
    layoutForWeb->addWidget(m_webControlPanel);

    this->setLayout(layoutForWeb);

    setContentDisplay(m_webDisplay);
    setAnnotationWidget(m_webDraw);
    setContextMenu(new ContextMenu(this));

    connect(m_webDisplay, SIGNAL(requestReadOnlyAnnotation(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));

    /*connect(m_webControlPanel, SIGNAL(locationChanged(QUrl)), m_webDisplay, SLOT(loadWebPage(QUrl)));
    connect(m_webControlPanel, SIGNAL(requestReadOnly(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));


    connect(m_webDisplay, SIGNAL(webPageLoadStarted()), m_webControlPanel, SLOT(loadStarted()));
    connect(m_webDisplay, SIGNAL(webPageLoadProgress(int)), m_webControlPanel, SLOT(loadProgress(int)));
    connect(m_webDisplay, SIGNAL(webPageLoadFinished(bool)), m_webControlPanel, SLOT(loadFinished(bool)));
    connect(m_webDisplay, SIGNAL(webPageUrlChanged(QUrl)), m_webControlPanel, SLOT(loadedPageChanged(QUrl)));*/

    m_annotationWidget->raise();
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

    // TODO connect QML slots and signals
    connect(m_panel, SIGNAL(back()), m_webDisplay, SLOT(back()));
    connect(m_panel, SIGNAL(next()), m_webDisplay, SLOT(forward()));
    connect(m_panel, SIGNAL(refresh()), m_webDisplay, SLOT(reload()));
    connect(m_panel, SIGNAL(webAnnotationStatus(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));

    /*signal back()
    signal next()
    signal refresh()*/

    // sync with webpage status
    // TODO sync other stats as well
    setWebGuiReadOnlyStatus(m_annotationWidget->isReadOnly());
}

void WebPageDisplayTask::deactivate()
{
    ContentDisplayTask::deactivate();

    // TODO disconnect QML slots and signals
    disconnect(m_panel, SIGNAL(back()), m_webDisplay, SLOT(back()));
    disconnect(m_panel, SIGNAL(next()), m_webDisplay, SLOT(forward()));
    disconnect(m_panel, SIGNAL(refresh()), m_webDisplay, SLOT(reload()));
    disconnect(m_panel, SIGNAL(webAnnotationStatus(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));
}

void WebPageDisplayTask::setWebGuiReadOnlyStatus(bool readOnly)
{
    QVariant bReadOnly = QVariant::fromValue(readOnly);

    QMetaObject::invokeMethod(m_panel, "setWebReadOnlyStatus", Q_ARG(QVariant, bReadOnly));
}
