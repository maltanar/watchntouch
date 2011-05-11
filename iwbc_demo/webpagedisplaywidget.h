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

public slots:
    void loadWebPage(QUrl newLocation);

private slots:
    void webPageLoadStartInternal();
    void webPageLoadFinishedInternal(bool ok);
    void scrollRequested ( int dx, int dy, const QRect & rectToScroll);


};

#endif // WEBPAGEDISPLAYWIDGET_H
