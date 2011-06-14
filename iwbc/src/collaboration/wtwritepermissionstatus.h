/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTWRITEPERMISSIONSTATUS_H
#define WTWRITEPERMISSIONSTATUS_H

#include <wtmessage.h>

class WTWritePermissionStatus : public WTMessage
{
public:
    explicit WTWritePermissionStatus();

    QByteArray serialize();
    void deserialize(QByteArray data);

    char getStatus();
    void setStatus(char status);

private:
    char status;
};

#endif // WTWRITEPERMISSIONSTATUS_H
