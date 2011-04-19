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

public slots:
    void loadWebPage(QUrl newLocation);

private slots:
    void webPageLoadFinished(bool ok);

};

#endif // WEBPAGEDISPLAYWIDGET_H
