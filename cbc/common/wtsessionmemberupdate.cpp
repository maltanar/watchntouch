#include "wtsessionmemberupdate.h"

WTSessionMemberUpdate::WTSessionMemberUpdate() :
    WTMessage()
{
    command = "SESSMUPD";
}

QByteArray WTSessionMemberUpdate::serialize()
{
    //Size of sessionName + size of updatedMemberCount + size of updateType
    // + total size of members' usernames and IPs
    msgSize += 8 + 4 + 1 + 8;
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    data.append(updateType);
    data.append(user.leftJustified(8, ' ').toAscii());
    return data;
}

void WTSessionMemberUpdate::deserialize(QByteArray data)
{
    char sessionName[9];
    char userName[9];
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName,8);
    sessionName[8] = '\0';
    dataStream.readRawData(&updateType, 1);
    dataStream.readRawData(userName, 8);
    userName[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();
    this->user = QString(userName).trimmed();
}

QString WTSessionMemberUpdate::getSessionName()
{
    return this->sessionName;
}

void WTSessionMemberUpdate::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}


void WTSessionMemberUpdate::setUpdateType(char updateType)
{
    this->updateType = updateType;
}

char WTSessionMemberUpdate::getUpdateType()
{
    return this->updateType;
}

QString WTSessionMemberUpdate::getUser()
{
    return user;
}

void WTSessionMemberUpdate::setUser(QString user)
{
    this->user = user;
}
