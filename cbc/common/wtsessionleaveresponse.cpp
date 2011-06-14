#include "wtsessionleaveresponse.h"

WTSessionLeaveResponse::WTSessionLeaveResponse() :
    WTMessage()
{
    command = "SESSQACK";
}

QByteArray WTSessionLeaveResponse::serialize()
{
    //Size of sessionName + size of result
    msgSize += 8 + 1;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    data.append(result);
    return data;
}

void WTSessionLeaveResponse::deserialize(QByteArray data)
{
    char sessionName[9];
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName, 8);
    sessionName[8] = '\0';
    dataStream.readRawData(&result,1);
    this->sessionName = QString(sessionName).trimmed();
}

char WTSessionLeaveResponse::getResult()
{
    return this->result;
}

void WTSessionLeaveResponse::setResult(char result)
{
    this->result = result;
}

QString WTSessionLeaveResponse::getSessionName()
{
    return this->sessionName;
}

void WTSessionLeaveResponse::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}
