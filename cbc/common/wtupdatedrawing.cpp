/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#include "wtupdatedrawing.h"

WTUpdateDrawing::WTUpdateDrawing() :
    WTMessage()
{
    command = "DRAWUPDT";
}

QByteArray WTUpdateDrawing::serialize()
{
    msgSize += 8 + picData.size();
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    data.append(picData);
    return data;
}

void WTUpdateDrawing::deserialize(QByteArray data)
{
    char sessionName[9];
    char *picData;
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName,8);
    sessionName[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();
    //Rest of the characters belong to picture data
    picData = new char[data.size()-(HEADER_SIZE+8)];
    dataStream.readRawData(picData, data.size() - (HEADER_SIZE+8));
    this->picData.setRawData(picData, data.size()-(HEADER_SIZE+8));
}

QString WTUpdateDrawing::getSessionName()
{
    return this->sessionName;
}

void WTUpdateDrawing::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QByteArray WTUpdateDrawing::getPicData()
{
    return picData;
}

void WTUpdateDrawing::setPicData(QByteArray picData)
{
    this->picData = picData;
}

