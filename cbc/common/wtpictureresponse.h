/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

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
