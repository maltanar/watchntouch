#ifndef WTSESSIONCREATERESPONSE_H
#define WTSESSIONCREATERESPONSE_H

#include "wtmessage.h"

class WTSessionCreateResponse : public WTMessage
{
public:
    explicit WTSessionCreateResponse();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

    QChar getResult();
    void setResult(QChar result);

    QString getPassword();
    void setPassword(QString password);

private:
    QString sessionName;
    //Result info:
    // 0 - Unsuccessful - Password might be wrong
    // 1 - Successful
    QChar result;
    QString password;
};

#endif // WTSESSIONCREATERESPONSE_H
