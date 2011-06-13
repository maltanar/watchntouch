#include "wtsessionjoinresponse.h"

WTSessionJoinResponse::WTSessionJoinResponse() :
    WTMessage(),
    userCount(0)
{
        command = "SESSRESP";
}

QByteArray WTSessionJoinResponse::serialize()
{
    //Size of result + size of sessionname + size of usercount + size of usernames and ips
    msgSize += 1 + 8 + 4 + 12 * userCount;
    QByteArray data = WTMessage::serialize();
    data.append(result);
    data.append(sessionName.leftJustified(8,' ').toAscii());
    data.append(QByteArray::fromRawData((const char *)&userCount,4));
    QHash<QString, long>::iterator iter;
    for (iter = users.begin(); iter != users.end(); iter++)
    {
        data.append(QString(iter.key()).leftJustified(8, ' ').toAscii());
        data.append(QByteArray::fromRawData((const char *)&(iter.value()),4));
    }
    return data;
}

void WTSessionJoinResponse::deserialize(QByteArray data)
{
    char sessionName[9];
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(&result,1);
    dataStream.readRawData(sessionName, 8);
    sessionName[8] = '\0';
    dataStream.readRawData((char *)&userCount, 4);
    for (unsigned int i = 0; i < userCount; i++)
    {
        char username[9];
        long userIP;
        dataStream.readRawData(username,8);
        username[8] = '\0';
        dataStream.readRawData((char *)&userIP, 4);
        users.insert(QString(username).trimmed(), userIP);
    }
    this->sessionName = QString(sessionName).trimmed();
}


void WTSessionJoinResponse::setResult(char result)
{
    this->result = result;
}

char WTSessionJoinResponse::getResult()
{
    return this->result;
}

void WTSessionJoinResponse::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QString WTSessionJoinResponse::getSessionName()
{
    return this->sessionName;
}

unsigned int WTSessionJoinResponse::getUserCount()
{
    return this->userCount;
}

void WTSessionJoinResponse::addUser(QString username, long userIP)
{
    this->userCount++;
    users.insert(username, userIP);
}

long WTSessionJoinResponse::getUserIP(QString username)
{
    QHash<QString,long>::const_iterator iter = users.find(username);
    return iter.value();
}

void WTSessionJoinResponse::setUsers(QHash<QString, long> users)
{
    this->users = users;
    this->userCount = users.size();
}

QHash<QString, long> WTSessionJoinResponse::getUsers()
{
    return this->users;
}

