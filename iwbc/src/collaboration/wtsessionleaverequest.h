#ifndef WTSESSIONLEAVEREQUEST_H
#define WTSESSIONLEAVEREQUEST_H

#include <wtmessage.h>

class WTSessionLeaveRequest : public WTMessage
{
public:
    explicit WTSessionLeaveRequest();

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setSessionName(QString sessionName);
    QString getSessionName();

private:
    QString sessionName;

};

#endif // WTSESSIONLEAVEREQUEST_H
