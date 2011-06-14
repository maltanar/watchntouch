/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef WTMESSAGE_H
#define WTMESSAGE_H

#include <QDataStream>
#include <QDebug>
#include <QStringList>

#define HEADER_SIZE 32

class WTMessage
{

public:
    explicit WTMessage();
    ~WTMessage();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray data);
    void setSrcUsername(QString username);
    QString getSrcUsername();
    void setDestUsername(QString username);
    QString getDestUsername();
    void setCommand(QString command);
    QString getCommand();
    void setVersion(QString version);
    QString getVersion();

protected:
    QString version;
    QString command;
    int msgSize;
    QString srcUsername;
    QString destUsername;


};

#endif // WTMESSAGE_H
