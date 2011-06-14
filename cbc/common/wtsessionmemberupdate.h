/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTSESSIONMEMBERUPDATE_H
#define WTSESSIONMEMBERUPDATE_H

#include <wtmessage.h>

#define UPDATE_SESSION_JOIN_BEGIN  0
#define UPDATE_SESSION_JOIN_END    1
#define UPDATE_SESSION_LEAVE       2

class WTSessionMemberUpdate : public WTMessage
{
public:
    explicit WTSessionMemberUpdate();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    char getUpdateType();
    void setUpdateType(char updateType);

    QString getUser();
    void setUser(QString user);

private:
    QString sessionName;
    char updateType;
    QString user;
};

#endif // WTSESSIONMEMBERUPDATE_H
