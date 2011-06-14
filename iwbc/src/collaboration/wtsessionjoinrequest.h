/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTSESSIONJOINREQUEST_H
#define WTSESSIONJOINREQUEST_H

#include <wtmessage.h>
#include <QCryptographicHash>


class WTSessionJoinRequest : public WTMessage
{
public:
    explicit WTSessionJoinRequest();

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setSessionName(QString sessionName);
    QString getSessionName();

    void setPassword(QString password);
    QString getPassword();

private:
    QString sessionName;
    QString password;

};

#endif // WTSESSIONJOINREQUEST_H
