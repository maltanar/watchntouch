/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTPEERHANDSHAKE_H
#define WTPEERHANDSHAKE_H

#include <wtmessage.h>

class WTPeerHandshake : public WTMessage
{
public:
    explicit WTPeerHandshake();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);
private:
    QString sessionName;

};

#endif // WTPEERHANDSHAKE_H
