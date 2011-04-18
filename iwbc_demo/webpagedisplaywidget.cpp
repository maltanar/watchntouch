#include "webpagedisplaywidget.h"
#include <QCryptographicHash>

WebpageDisplayWidget::WebpageDisplayWidget(QWidget *parent) :
    ContentDisplay(parent)
{
    mWebView = new QWebView(this);
}

bool WebpageDisplayWidget::selectContent(QString location)
{
    mUrlString = location;
    mWebView->load(QUrl(location));
    generateContentIdentifier();
    emit contentChanged(getContentIdentifier());
    // TODO currently each webpage is a single context - a more sophisticated system needed?
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
