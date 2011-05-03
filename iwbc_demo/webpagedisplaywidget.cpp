#include "webpagedisplaywidget.h"
#include <QCryptographicHash>
#include <QVBoxLayout>
#include "recentlyused.h"

WebpageDisplayWidget::WebpageDisplayWidget(QWidget *parent) :
    ContentDisplay(parent)
{
    QVBoxLayout * layout = new QVBoxLayout();
    mWebView = new QWebView(this);
    layout->addWidget(mWebView);
    setLayout(layout);

    connect(mWebView, SIGNAL(loadFinished(bool)), this, SLOT(webPageLoadFinishedInternal(bool)));
    connect(mWebView, SIGNAL(loadProgress(int)), this, SIGNAL(webPageLoadProgress(int)));
    connect(mWebView, SIGNAL(loadStarted()), this, SIGNAL(webPageLoadStarted()));
    connect(mWebView, SIGNAL(loadFinished(bool)), this, SIGNAL(webPageLoadFinished(bool)));
    connect(mWebView, SIGNAL(urlChanged(QUrl)), this, SIGNAL(webPageUrlChanged(QUrl)));
}

bool WebpageDisplayWidget::selectContent(QString location)
{
    mUrlString = location;
    mWebView->load(QUrl(mUrlString));
    // emit empty content and context message since from this point on to when the loading is finished,
    // we're in limbo and should not really display any annotation
    emit contentChanged("");
    emit contextChanged("");
    emit requestReadOnlyAnnotation(true);
    // TODO we should actually return the value for the loadFinished OK parameter here
    return true;
}

QString WebpageDisplayWidget::getContentContext()
{
    // TODO currently each webpage is a single context - a more sophisticated system needed?
    return "1";
}

void WebpageDisplayWidget::generateContentIdentifier()
{
    contentMD5 = QString(QCryptographicHash::hash(mUrlString.toAscii(), QCryptographicHash::Md5).toHex().constData());
}

void WebpageDisplayWidget::loadWebPage(QUrl newLocation)
{
    selectContent(newLocation.toString());
}

void WebpageDisplayWidget::webPageLoadFinishedInternal(bool ok)
{
    if(ok) {
        recentlyUsed->addRecentItem(mWebView->title(), mUrlString);
        generateContentIdentifier();
        emit contentChanged(getContentIdentifier());
        // TODO currently each webpage is a single context - a more sophisticated system needed?
        emit contextChanged("1");
        emit requestReadOnlyAnnotation(false);
    } else {
        displayErrorMessage("Could not load requested web page");
        mUrlString = "";
    }
}
