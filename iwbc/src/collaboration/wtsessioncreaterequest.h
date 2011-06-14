/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTSESSIONCREATEREQUEST_H
#define WTSESSIONCREATEREQUEST_H

#include <wtmessage.h>
#include <QCryptographicHash>

class WTSessionCreateRequest : public WTMessage
{
public:
    explicit WTSessionCreateRequest();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    QString getPassword();
    void setPassword(QString password);

private:
    QString sessionName;
    QString password;

};

#endif // WTSESSIONCREATEREQUEST_H
