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

    void setExternalViewportSize(QSize newSize);

private:
    QWebView * mWebView;
    QString mUrlString;
    QSize m_externalViewportSize;

signals:
    void webPageLoadProgress(int progress);
    void webPageLoadStarted();
    void webPageLoadFinished(bool ok);
    void webPageUrlChanged(QUrl newUrl);
    void scrollRequested ( int dx, int dy);

public slots:
    void loadWebPage(QString newLocation);
    void forward();
    void back();
    void reload();
    void stop();

private slots:
    void webPageLoadStartInternal();
    void webPageLoadFinishedInternal(bool ok);


};

#endif // WEBPAGEDISPLAYWIDGET_H
