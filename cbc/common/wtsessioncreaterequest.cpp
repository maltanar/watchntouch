#include "wtsessioncreaterequest.h"


WTSessionCreateRequest::WTSessionCreateRequest() :
    WTMessage()
{
    command = "SESCRTRQ";
}


QByteArray WTSessionCreateRequest::serialize()
{
    //Message size is the size of common header + session name + password in md5
    msgSize += 8 + 32;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    //Send md5 code of the password
    data.append(password.leftJustified(32, ' ').toAscii());
    return data;
}

void WTSessionCreateRequest::deserialize(QByteArray data)
{
    char sessionName[9];
    char password[33];

    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username as they have already been deserialized above
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName, 8);
    sessionName[8] = '\0';
    dataStream.readRawData(password, 32);
    password[32] = '\0';
    this->sessionName = QString(sessionName).trimmed();
    this->password = QString(password).trimmed();
}

QString WTSessionCreateRequest::getSessionName()
{
    return this->sessionName;
}

void WTSessionCreateRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QString WTSessionCreateRequest::getPassword()
{
    return this->password;
}

void WTSessionCreateRequest::setPassword(QString password)
{
    //Convert the password into md5
    this->password = QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Md5);
}
