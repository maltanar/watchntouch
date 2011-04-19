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

}

bool WebpageDisplayWidget::selectContent(QString location)
{
    mUrlString = location;
    mWebView->load(QUrl(mUrlString));
    // TODO use webpage title in recent item identifier
    recentlyUsed->addRecentItem("Web Site", location);
    generateContentIdentifier();
    emit contentChanged(getContentIdentifier());
    // TODO currently each webpage is a single context - a more sophisticated system needed?
    // TODO emit signals when the page has finished loading maybe?
    emit contextChanged("1");
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
