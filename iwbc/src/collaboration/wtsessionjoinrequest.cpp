#include "wtsessionjoinrequest.h"

WTSessionJoinRequest::WTSessionJoinRequest() :
    WTMessage()
{
    command = "SESSJOIN";
}

QByteArray WTSessionJoinRequest::serialize()
{
    //Size of sessionName + size of encrypted password
    msgSize += 8 + 32;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8,' ').toAscii());
    data.append(password.leftJustified(32, ' ').toAscii());
    return data;
}

void WTSessionJoinRequest::deserialize(QByteArray data)
{
    QDataStream dataStream(data);
    char password[33];
    char sessionName[9];
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName, 8);
    dataStream.readRawData(password, 32);
    sessionName[8] = '\0';
    password[32] = '\0';
    this->sessionName = QString(sessionName).trimmed();
    this->password = QString(password).trimmed();
}

void WTSessionJoinRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QString WTSessionJoinRequest::getSessionName()
{
    return this->sessionName;
}

void WTSessionJoinRequest::setPassword(QString password)
{
    //Store the password as encrypted
    this->password = password;
    //this->password = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Md5);
}

QString WTSessionJoinRequest::getPassword()
{
    return password;
}


