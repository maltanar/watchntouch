/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#include "wtsessioncreateresponse.h"

WTSessionCreateResponse::WTSessionCreateResponse() :
    WTMessage()
{
        command = "SESCRTRS";
}


QByteArray WTSessionCreateResponse::serialize()
{
    //Size of common header + result + session name + encrypted password
    msgSize += 8 + 1 + 32;
    QByteArray data = WTMessage::serialize();
    data.append(result);
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    data.append(password.leftJustified(32, ' ').toAscii());
    return data;
}

void WTSessionCreateResponse::deserialize(QByteArray data)
{
    char sessionName[9];
    char password[33];
    char result;

    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username as they have already been deserialized above
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(&result, 1);
    dataStream.readRawData(sessionName, 8);
    sessionName[8] = '\0';
    dataStream.readRawData(password, 32);
    password[32] = '\0';

    this->sessionName = QString(sessionName).trimmed();
    this->result = QChar(result);
    this->password = QString(password).trimmed();
}

QString WTSessionCreateResponse::getSessionName()
{
    return this->sessionName;
}

void WTSessionCreateResponse::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QChar WTSessionCreateResponse::getResult()
{
    return this->result;
}

void WTSessionCreateResponse::setResult(QChar result)
{
    this->result = result;
}

QString WTSessionCreateResponse::getPassword()
{
    return this->password;
}

void WTSessionCreateResponse::setPassword(QString password)
{
    //It is assumed here that the password is already encrypted
    this->password = password;
}
