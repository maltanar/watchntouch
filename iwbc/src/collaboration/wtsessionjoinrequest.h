#ifndef WTSESSIONJOINREQUEST_H
#define WTSESSIONJOINREQUEST_H

#include <wtmessage.h>
#include <QCryptographicHash>


class WTSessionJoinRequest : public WTMessage
{
public:
    explicit WTSessionJoinRequest();

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setSessionName(QString sessionName);
    QString getSessionName();

    void setPassword(QString password);
    QString getPassword();

private:
    QString sessionName;
    QString password;

};

#endif // WTSESSIONJOINREQUEST_H
