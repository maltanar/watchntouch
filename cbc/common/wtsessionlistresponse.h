/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTSESSIONLISTRESPONSE_H
#define WTSESSIONLISTRESPONSE_H

#include <wtmessage.h>

class WTSessionListResponse : public WTMessage
{
public:
    explicit WTSessionListResponse();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QStringList getSessionList();
    void setSessionList(QStringList &sessionList);
    void addSession(QString sessionName);

private:
    uint sesscnt;
    QStringList sessionList;

};

#endif // WTSESSIONLISTRESPONSE_H
