#include "wtpictureresponse.h"

WTPictureResponse::WTPictureResponse() :
    WTMessage()
{
    command = "COLSTARS";
}

QByteArray WTPictureResponse::serialize()
{
    //size of sessionName + size of picdata
    msgSize += 8 + picData.size();
    QByteArray data = WTMessage::serialize();
    data.append(sessionName.leftJustified(8, ' ').toAscii());
    data.append(picData);
    return data;
}

void WTPictureResponse::deserialize(QByteArray data)
{
    char sessionName[9];
    char *picData;
    QDataStream dataStream(data);
    WTMessage::deserialize(data);
    //Skip header and username
    dataStream.skipRawData(HEADER_SIZE);
    dataStream.readRawData(sessionName, 8);
    sessionName[8] = '\0';
    this->sessionName = QString(sessionName).trimmed();

    picData = new char[data.size()-(HEADER_SIZE + 8)];
    dataStream.readRawData(picData, data.size() - (HEADER_SIZE + 8));
    this->picData.setRawData(picData, data.size() - (HEADER_SIZE + 8));
}

QString WTPictureResponse::getSessionName()
{
    return this->sessionName;
}

void WTPictureResponse::setSessionName(QString sessionName)
{
    this->sessionName = sessionName;
}

QByteArray WTPictureResponse::getPicData()
{
    return picData;
}

void WTPictureResponse::setPicData(QByteArray picData)
{
    this->picData = picData;
}
