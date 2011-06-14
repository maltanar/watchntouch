/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTSESSIONLEAVEREQUEST_H
#define WTSESSIONLEAVEREQUEST_H

#include <wtmessage.h>

class WTSessionLeaveRequest : public WTMessage
{
public:
    explicit WTSessionLeaveRequest();

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setSessionName(QString sessionName);
    QString getSessionName();

private:
    QString sessionName;

};

#endif // WTSESSIONLEAVEREQUEST_H
