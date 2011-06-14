#ifndef WTUPDATEDRAWINGSERVER_H
#define WTUPDATEDRAWINGSERVER_H

#include "wtmessage.h"

class WTUpdateDrawingServer : public WTMessage
{
public:
    explicit WTUpdateDrawingServer();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    QByteArray getPicData();
    void setPicData(QByteArray picData);

private:
    QString sessionName;
    QByteArray picData;

};

#endif // WTUPDATEDRAWINGSERVER_H
