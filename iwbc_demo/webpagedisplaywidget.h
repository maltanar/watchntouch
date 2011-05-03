#ifndef WEBPAGEDISPLAYWIDGET_H
#define WEBPAGEDISPLAYWIDGET_H

#include <QtWebKit/QWebView>
#include "contentdisplay.h"

class WebpageDisplayWidget : public ContentDisplay
{
    Q_OBJECT
public:
    explicit WebpageDisplayWidget(QWidget *parent = 0);

    bool selectContent(QString location);
    QString getContentContext();
    void generateContentIdentifier();

private:
    QWebView * mWebView;
    QString mUrlString;

signals:
    void webPageLoadProgress(int progress);
    void webPageLoadStarted();
    void webPageLoadFinished(bool ok);
    void webPageUrlChanged(QUrl newUrl);

public slots:
    void loadWebPage(QUrl newLocation);

private slots:
    void webPageLoadFinishedInternal(bool ok);


};

#endif // WEBPAGEDISPLAYWIDGET_H
