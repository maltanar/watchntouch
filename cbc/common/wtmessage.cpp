#include "wtmessage.h"

WTMessage::WTMessage() :
        msgSize(HEADER_SIZE) //Constant header size + username
{
        version = "WTC1";
}

WTMessage::~WTMessage()
{

}

QByteArray WTMessage::serialize()
{
    QByteArray data(version.toAscii());
    data.append(command.toAscii());
    data.append(QByteArray::fromRawData((const char*)&msgSize, 4));
    data.append(srcUsername.leftJustified(8, ' ').toAscii());
    data.append(destUsername.leftJustified(8, ' ').toAscii());
    return data;
}

void WTMessage::deserialize(QByteArray data)
{
    //Assuming the header is always of the same size.
    char version[5];
    char command[9];
    char srcUsername[9];
    char destUsername[9];
    QDataStream dataStream(data);
    dataStream.readRawData(version, 4);
    version[4] = '\0';
    dataStream.readRawData(command, 8);
    command[8] = '\0';
    dataStream.readRawData((char *)&msgSize, 4);
    dataStream.readRawData(srcUsername, 8);
    srcUsername[8] = '\0';
    dataStream.readRawData(destUsername, 8);
    destUsername[8] = '\0';
    this->version = QString(version);
    this->command = QString(command);
    this->srcUsername = QString(srcUsername).trimmed();
    this->destUsername = QString(destUsername).trimmed();
}


void WTMessage::setSrcUsername(QString username)
{
    this->srcUsername = username;
}

QString WTMessage::getSrcUsername()
{
    return this->srcUsername;
}

void WTMessage::setDestUsername(QString username)
{
    this->destUsername = username;
}

QString WTMessage::getDestUsername()
{
    return this->destUsername;
}

void WTMessage::setCommand(QString command)
{
    this->command = command;
}

QString WTMessage::getCommand()
{
    return this->command;
}

void WTMessage::setVersion(QString version)
{
    this->version = version;
}

QString WTMessage::getVersion()
{
    return this->version;
}
