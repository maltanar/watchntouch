/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#include "wtpicturerequest.h"

WTPictureRequest::WTPictureRequest() :
    WTMessage()
{
    command = "COLSTARQ";
}

QByteArray WTPictureRequest::serialize()
{
    //Size of sessionName
    msgSize += 8;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    return data;
}

void WTPictureRequest::deserialize(QByteArray data)
{
    char sessionName[9];
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName,8);
    sessionName[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();
}

QString WTPictureRequest::getSessionName()
{
    return this->sessionName;
}

void WTPictureRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

