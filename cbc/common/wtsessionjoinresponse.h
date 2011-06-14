#ifndef WTSESSIONJOINRESPONSE_H
#define WTSESSIONJOINRESPONSE_H

#include <wtmessage.h>

class WTSessionJoinResponse : public WTMessage
{
public:
    explicit WTSessionJoinResponse();

    QByteArray serialize();
    void deserialize(QByteArray data);

    void setResult(char result);
    char getResult();
    void setSessionName(QString sessionName);
    QString getSessionName();
    unsigned int getUserCount();
    void addUser(QString username, long userIP);
    long getUserIP(QString username);
    void setUsers(QHash<QString, long> users);
    QHash<QString, long> getUsers();

private:
    char result;
    QString sessionName;
    unsigned int userCount;
    QHash<QString, long> users;
};

#endif // WTSESSIONJOINRESPONSE_H
