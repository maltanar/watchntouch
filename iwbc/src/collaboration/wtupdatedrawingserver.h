/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTUPDATEDRAWINGSERVER_H
#define WTUPDATEDRAWINGSERVER_H

#include "wtmessage.h"

class WTUpdateDrawingServer : public WTMessage
{
public:
    explicit WTUpdateDrawingServer();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    QByteArray getPicData();
    void setPicData(QByteArray picData);

private:
    QString sessionName;
    QByteArray picData;

};

#endif // WTUPDATEDRAWINGSERVER_H
