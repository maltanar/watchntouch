#include "googledocsaccess.h"

#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QStringList>
#include <QDomDocument>

#include <QFile>
#include <QMessageBox>
#include <QDebug>


GoogleDocsAccess::GoogleDocsAccess(QObject *parent) :
    QObject(parent)
{
    m_state = GDAS_NOTSTARTED;
    m_accessManager = new QNetworkAccessManager(this);
}

GoogleDocsAccess::~GoogleDocsAccess()
{
    m_accessManager->deleteLater();
}

bool GoogleDocsAccess::login(QString userName, QString password)
{
    // TODO set constant URLs as definable
    // TODO URL encoding needed?
    QNetworkRequest request(QUrl("https://www.google.com/accounts/ClientLogin"));
    QString loginString("accountType=HOSTED_OR_GOOGLE&Email=%1&Passwd=%2&service=writely&source=WatchAndTouchv1.0");

    m_userName = userName;
    m_password = password;

    loginString = loginString.arg(m_userName).arg(m_password);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = m_accessManager->post(request, loginString.toAscii());

    // wait until we receive a reply
    // TODO implement timeout
    QEventLoop eLoop;
    connect( m_accessManager, SIGNAL(finished(QNetworkReply *)), &eLoop, SLOT(quit()));
    eLoop.exec();

    // TODO process the result
    QByteArray resultBytes(reply->readAll());
    QString resultString(resultBytes);
    int resultCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(resultCode == 200)
    {
        // TODO login succeeded, get token
        QStringList resultLines = resultString.split("\n");
        QString currentLine;

        for(int i = 0; i < resultLines.count(); i++)
        {
            currentLine = resultLines.at(i);
            if(currentLine.left(4) == "Auth")
            {
                // strip the Auth= part, the remainder is the access token
                m_accessToken = currentLine.mid(5).trimmed();
                m_state = GDAS_VERIFIED;
                break;
            }
        }
        return true;
    }
    else if(resultCode == 203)
    {
        // TODO investigate failure, maybe captcha needed
        return false;
    }
    else
    {
        // TODO unknown failure?
        return false;
    }
}

QString GoogleDocsAccess::getCaptchaImageURL()
{
    // TODO implement getCaptchaImageURL function
    if(m_state != GDAS_NEEDCAPTCHA)
        return "";

    return "";
}

bool GoogleDocsAccess::loginWithCaptcha(QString captchaCode)
{
    // TODO implement loginWithCaptcha function
    if(m_state != GDAS_NEEDCAPTCHA)
        return false;
    return false;
}

bool GoogleDocsAccess::fetchList()
{
    // TODO fix constant URLs
    QNetworkRequest request(QUrl("https://docs.google.com/feeds/default/private/full/-/presentation"));
    QString queryString("GoogleLogin auth=%1");

    queryString = queryString.arg(m_accessToken);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(QByteArray("Authorization"), queryString.toAscii());
    request.setRawHeader(QByteArray("GData-Version"), QByteArray("3.0"));

    QNetworkReply *reply = m_accessManager->get(request);

    // wait until we receive a reply
    // TODO implement timeout
    QEventLoop eLoop;
    connect( m_accessManager, SIGNAL(finished(QNetworkReply *)), &eLoop, SLOT(quit()));
    eLoop.exec();

    // process returned data
    // TODO handle errors
    QString resultString(reply->readAll());

    QDomDocument doc;
    doc.setContent(resultString);

    QDomElement rootElement = doc.documentElement();
    QDomNodeList entryList = rootElement.elementsByTagName("entry");

    documentList.clear();
    GoogleDocEntry entry;
    for(int i = 0; i < entryList.count(); i++)
    {
        entry.importFromXMLNode(entryList.at(i));
        documentList.append(entry);
    }

    return true;
}

QList<GoogleDocEntry> GoogleDocsAccess::getDocumentList()
{
    return documentList;
}

void GoogleDocsAccess::logout()
{
    // TODO implement logout function - nothing to do here except destroy the info?
    m_userName = "";
    m_password = "";
    m_accessToken = "";
    m_state = GDAS_NOTSTARTED;
}

bool GoogleDocsAccess::downloadPresentation(GoogleDocEntry presentationEntry, QString fileName, QString format)
{


    return true;
}

bool GoogleDocsAccess::downloadPresentation(QString presentationResourceID, QString fileName, QString format)
{
    // TODO fix constant URLs
    // TODO check format validity
    QString requestUrl = "https://docs.google.com/feeds/download/documents/Export?docID=%1&exportFormat=%2&format=%2";
    requestUrl = requestUrl.arg(presentationResourceID).arg(format);
    qWarning() << requestUrl;

    QNetworkRequest request;
    QString queryString("GoogleLogin auth=%1");

    queryString = queryString.arg(m_accessToken);

    request.setUrl(QUrl(requestUrl));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(QByteArray("Authorization"), queryString.toAscii());
    request.setRawHeader(QByteArray("GData-Version"), QByteArray("3.0"));

    QNetworkReply *reply = m_accessManager->get(request);

    // wait until we receive a reply
    // TODO implement timeout
    QEventLoop eLoop;
    connect( m_accessManager, SIGNAL(finished(QNetworkReply *)), &eLoop, SLOT(quit()));
    eLoop.exec();

    // process returned data
    // TODO handle errors
    QFile outputFile(fileName);
    outputFile.open(QIODevice::ReadWrite);
    outputFile.write(reply->readAll());
    outputFile.close();
}
