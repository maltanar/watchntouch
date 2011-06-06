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

    // connect signals and slots for web
    // TODO connect QML signals and slots instead
    connect(m_webControlPanel, SIGNAL(locationChanged(QUrl)), m_webDisplay, SLOT(loadWebPage(QUrl)));
    connect(m_webControlPanel, SIGNAL(requestReadOnly(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));

    connect(m_webDisplay, SIGNAL(requestReadOnlyAnnotation(bool)), m_webDraw, SLOT(requestReadOnlyStatus(bool)));
    connect(m_webDisplay, SIGNAL(webPageLoadStarted()), m_webControlPanel, SLOT(loadStarted()));
    connect(m_webDisplay, SIGNAL(webPageLoadProgress(int)), m_webControlPanel, SLOT(loadProgress(int)));
    connect(m_webDisplay, SIGNAL(webPageLoadFinished(bool)), m_webControlPanel, SLOT(loadFinished(bool)));
    connect(m_webDisplay, SIGNAL(webPageUrlChanged(QUrl)), m_webControlPanel, SLOT(loadedPageChanged(QUrl)));

    m_annotationWidget->raise();
}

ContentType WebPageDisplayTask::getContentType()
{
    return CONTENTTYPE_WEBPAGE;
}
