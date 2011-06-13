#include "wtsessionlistresponse.h"

WTSessionListResponse::WTSessionListResponse() :
    WTMessage(),
    sesscnt(0)
{
    command = "SESSLRES";
}

QByteArray WTSessionListResponse::serialize()
{
    //size of sesscnt + total size of session names
    msgSize += 4 + sesscnt * 8;
    QByteArray data = WTMessage::serialize();
    data.append(QByteArray::fromRawData((const char *)&sesscnt, 4));

    int size = sessionList.size();
    for (int i = 0; i < size; i++)
    {
        data.append(((QString)sessionList.at(i)).leftJustified(8, ' ').toAscii());
    }
    return data;
}

void WTSessionListResponse::deserialize(QByteArray data)
{
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData((char *)&sesscnt, 4);
    qWarning() << sesscnt;
    for (unsigned int i = 0; i < sesscnt; i++)
    {
        char sessionName[9];
        dataStream.readRawData(sessionName, 8);
        sessionName[8] = '\0';
        sessionList.append(QString(sessionName).trimmed());
    }
}

QStringList WTSessionListResponse::getSessionList()
{
    return sessionList;
}

void WTSessionListResponse::setSessionList(QStringList &sessionList)
{
    this->sessionList = sessionList;
    sesscnt = sessionList.count();
}

void WTSessionListResponse::addSession(QString sessionName)
{
    sesscnt++;
    sessionList.append(sessionName);
}



