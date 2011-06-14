#include "wtpeerhandshake.h"

WTPeerHandshake::WTPeerHandshake() :
    WTMessage()
{
    command = "HELLOPAL";
}


QByteArray WTPeerHandshake::serialize()
{
    //Size of sessionName
    msgSize += 8;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    return data;
}


void WTPeerHandshake::deserialize(QByteArray data)
{
    char sessionName[9];
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip the header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName, 8);
    sessionName[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();
}

QString WTPeerHandshake::getSessionName()
{
    return this->sessionName;
}

void WTPeerHandshake::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}
