#include "wtsessionleaverequest.h"

WTSessionLeaveRequest::WTSessionLeaveRequest() :
    WTMessage()
{
    command = "SESSQUIT";
}

QByteArray WTSessionLeaveRequest::serialize()
{
    //Size of sessionName
    msgSize += 8;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    return data;
}

void WTSessionLeaveRequest::deserialize(QByteArray data)
{
    char sessionName[9];
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName, 8);
    sessionName[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();
}

QString WTSessionLeaveRequest::getSessionName()
{
    return this->sessionName;
}

void WTSessionLeaveRequest::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}
