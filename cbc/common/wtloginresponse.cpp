/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#include "wtloginresponse.h"

WTLoginResponse::WTLoginResponse() :
    WTMessage()
{
    command = "LOGINRES";
}

QByteArray WTLoginResponse::serialize()
{
    //Size of result + size of msg size + size of msg
    msgSize += 1 + 2 + infomsg.length();
    QByteArray data = WTMessage::serialize();
    data.append(result);
    data.append(QByteArray::fromRawData((const char*)&infomsgSize,2));
    data.append(infomsg.toAscii());
    return data;
}

void WTLoginResponse::deserialize(QByteArray data)
{
    QDataStream dataStream(data);
    char result;
    char *infomsg;
    WTMessage::deserialize(data);

    //Skip header and usernames
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(&result, 1);
    dataStream.readRawData((char *)&infomsgSize, 2);
    infomsg = new char[infomsgSize+1];
    dataStream.readRawData(infomsg, infomsgSize);
    infomsg[infomsgSize] = '\0';
    this->infomsg = QString(infomsg);
}

void WTLoginResponse::setInfomsg(QString msg)
{
    this->infomsg = msg;
    infomsgSize = msg.size();
}

QString WTLoginResponse::getInfomsg()
{
    return this->infomsg;
}

void WTLoginResponse::setResult(char result)
{
    this->result = result;
}

char WTLoginResponse::getResult()
{
    return this->result;
}
