#include "webpagedisplaytask.h"

WebPageDisplayTask::WebPageDisplayTask(QWidget *parent) :
    ContentDisplayTask(parent)
{
    webCanvas = new QWidget(this);

    webDisplay = new WebpageDisplayWidget(webCanvas);
    webDraw = new AnnotationWidget(webCanvas);

    webControlPanel = new WebControlPanel(this);

    QStackedLayout *layoutForWebCanvas = new QStackedLayout();
    layoutForWebCanvas->addWidget(webDisplay);
    layoutForWebCanvas->addWidget(webDraw);
    layoutForWebCanvas->setStackingMode(QStackedLayout::StackAll);
    layoutForWebCanvas->setAlignment(display, Qt::AlignHCenter);
    layoutForWebCanvas->setAlignment(draw, Qt::AlignHCenter);

    webCanvas->setLayout(layoutForWebCanvas);

    QVBoxLayout *layoutForWeb = new QVBoxLayout();
    layoutForWeb->addWidget(webCanvas);
    layoutForWeb->addWidget(webControlPanel);

    this->setLayout(layoutForWeb);

    // connect signals and slots for web

    connect(webControlPanel, SIGNAL(locationChanged(QUrl)), webDisplay, SLOT(loadWebPage(QUrl)));
    connect(webControlPanel, SIGNAL(requestReadOnly(bool)), webDraw, SLOT(requestReadOnlyStatus(bool)));

    connect(webDisplay, SIGNAL(requestReadOnlyAnnotation(bool)), webDraw, SLOT(requestReadOnlyStatus(bool)));
    connect(webDisplay, SIGNAL(webPageLoadStarted()), webControlPanel, SLOT(loadStarted()));
    connect(webDisplay, SIGNAL(webPageLoadProgress(int)), webControlPanel, SLOT(loadProgress(int)));
    connect(webDisplay, SIGNAL(webPageLoadFinished(bool)), webControlPanel, SLOT(loadFinished(bool)));
    connect(webDisplay, SIGNAL(webPageUrlChanged(QUrl)), webControlPanel, SLOT(loadedPageChanged(QUrl)));
}
