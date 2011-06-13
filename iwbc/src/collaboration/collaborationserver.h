#ifndef COLLABORATIONSERVER_H
#define COLLABORATIONSERVER_H

#include <QObject>
#include <QHash>
#include <QList>

#include "protocolhandler.h"
#include "collaborationsession.h"

//TODO move into a common header file with client
#define COLLABORATION_SERVER_NAME       "$SERVER$"
#define SERVICE_BROADCAST_PERIOD_MS     1000
#define SERVICE_BROADCAST_PORT          45455

class CollaborationServer : public QObject
{
    Q_OBJECT
public:
    explicit CollaborationServer(QObject *parent = 0);

    void setProtocolHandler(ProtocolHandler * newProtocolHandler);
    ProtocolHandler * getProtocolHandler();

    void setServerUserName(QString newUserName);
    QString getServerUserName();

protected:
    QString m_serverUserName;
    QList<QString> m_userList;
    QList<QString> m_sessionList;
    ProtocolHandler * m_protocolHandler;
    QUdpSocket serviceBroadcastSocket;
    QTimer serviceBroadcastTimer;
    QHash<QString, CollaborationSession *> m_sessionData;


signals:
    void sendLoginResponse(QString destUserName, char result, QString infoMsg);
    void sendPictureResponse(QString destUserName, QString sessionName, QByteArray picData);
    void sendSessionCreateResponse(QString destUserName, QString sessionName, QChar result, QString password);
    void sendSessionJoinResponse(QString destUserName, QString sessionName, char result, QHash<QString, long> users);
    void sendSessionLeaveResponse(QString destUserName, QString sessionName, char result);
    void sendSessionListResponse(QString destUserName, QStringList sessionList);
    void sendSessionMemberUpdate(QString destUserName, QString sessionName, char updateType, QString users);
    void sendWritePermissionStatus(QString destUserName, QChar status);

public slots:
    void receivedLoginRequest(QString userName);
    void receivedLogoutRequest(QString userName);
    void receivedPictureRequest(QString userName, QString sessionName);
    void receivedSessionCreateRequest(QString userName, QString sessionName, QString password);
    void receivedSessionJoinRequest(QString userName, QString sessionName, QString password);
    void receivedSessionLeaveRequest(QString userName, QString sessionName);
    void receivedSessionListRequest(QString userName);
    void receivedUpdateDrawingServer(QString userName, QString sessionName, QByteArray picData);
    void receivedWritePermissionRequest(QString userName);

private slots:
    void serviceBroadcastTimeout();
};

#endif // COLLABORATIONSERVER_H
