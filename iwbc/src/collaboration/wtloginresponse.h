#ifndef WTLOGINRESPONSE_H
#define WTLOGINRESPONSE_H

#include <wtmessage.h>

class WTLoginResponse : public WTMessage
{
public:
    explicit WTLoginResponse();

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setInfomsg(QString msg);
    QString getInfomsg();

    void setResult(char result);
    char getResult();

private:
    char result;
    QString infomsg;
    short infomsgSize;
};

#endif // WTLOGINRESPONSE_H
