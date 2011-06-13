#include "wtwritepermissionstatus.h"

WTWritePermissionStatus::WTWritePermissionStatus() :
    WTMessage()
{
    command = "WRTPRMST";
}

QByteArray WTWritePermissionStatus::serialize()
{
    //Size of status
    msgSize += 1;
    QByteArray data = WTMessage::serialize();
    data.append(status);
    return data;
}

void WTWritePermissionStatus::deserialize(QByteArray data)
{
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(&status, 1);
}

char WTWritePermissionStatus::getStatus()
{
    return this->status;
}

void WTWritePermissionStatus::setStatus(char status)
{
    this->status = status;
}
