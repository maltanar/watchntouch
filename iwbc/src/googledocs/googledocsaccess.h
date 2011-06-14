/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef GOOGLEDOCSACCESS_H
#define GOOGLEDOCSACCESS_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QHash>
#include <QList>
#include "googledocentry.h"

// TODO devise a more generic error handling system

class GoogleDocsAccess : public QObject
{
    Q_OBJECT
public:
    explicit GoogleDocsAccess(QObject *parent = 0);
    ~GoogleDocsAccess();

    bool login(QString userName, QString password);
    QString getCaptchaImageURL();
    bool loginWithCaptcha(QString captchaCode);

    bool fetchList();
    QList<GoogleDocEntry> getDocumentList();

    bool downloadPresentation(GoogleDocEntry presentationEntry, QString fileName, QString format);
    bool downloadPresentation(QString presentationResourceID, QString fileName, QString format);

    void logout();

signals:

public slots:

private:
    QString m_userName;
    QString m_password;
    QNetworkAccessManager *m_accessManager;
    QString m_captchaUrl;
    QString m_accessToken;
    QHash<QString, GoogleDocEntry> m_docs;
    QList<GoogleDocEntry> documentList;
    enum GoogleDocsAccessState {GDAS_NOTSTARTED, GDAS_NEEDCAPTCHA, GDAS_VERIFIED, GDAS_LOGGEDOUT} m_state;

};

#endif // GOOGLEDOCSACCESS_H
