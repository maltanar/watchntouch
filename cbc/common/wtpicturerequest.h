#ifndef WTPICTUREREQUEST_H
#define WTPICTUREREQUEST_H

#include <wtmessage.h>

class WTPictureRequest : public WTMessage
{
public:
    explicit WTPictureRequest();

    QByteArray serialize();
    void deserialize(QByteArray data);

    QString getSessionName();
    void setSessionName(QString sessionName);

private:
    QString sessionName;

};

#endif // WTPICTUREREQUEST_H
