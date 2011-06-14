#ifndef WTPICTURERESPONSE_H
#define WTPICTURERESPONSE_H

#include <wtmessage.h>

class WTPictureResponse : public WTMessage
{
public:
    explicit WTPictureResponse();

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

#endif // WTPICTURERESPONSE_H
