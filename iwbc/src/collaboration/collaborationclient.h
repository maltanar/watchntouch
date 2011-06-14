/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef COLLABORATIONCLIENT_H
#define COLLABORATIONCLIENT_H

#include <QObject>
#include <QHash>
#include <QStringList>

#include <protocolhandler.h>
#include <collaborationsession.h>

#define SERVICE_BROADCAST_PORT 45455

#define JOIN_SESSION_STATE 1
#define JOIN_SESSION_PEERHANDHAKE_COMPLETED 2
#define JOIN_SESSION_COMPLETED 3
#define MEMBER_UPDATE_JOIN_BEGIN_RECEIVED 4
#define MEMBER_UPDATE_JOIN_END_RECEIVED 5

class CollaborationClient : public QObject
{
    Q_OBJECT
public:
    explicit CollaborationClient(QObject *parent = 0);

    void setProtocolHandler(ProtocolHandler * newProtocolHandler);
    ProtocolHandler * getProtocolHandler();

    void loginToServer(QHostAddress serverAddress, QString serverName, QString userName);
    void refreshSessionList();
    void joinSession(QString sessionName, QString password);
    void sendDrawing(QString sessionName, QByteArray picData);
    void createSession(QString sessionName, QString password);
    void leaveSession(QString sessionName);

    //TODO To be removed
    QString getActiveSession();

    void sendBufferedData(QString sessionName, QString user);

private:
    QString m_serverName;
    QStringList m_userList;
    QStringList m_sessionList;
    ProtocolHandler *m_protocolHandler;
    QHash<QString, CollaborationSession *> m_collaborationSessions;
    QHash<QString, QString> m_pendingHandshakes;

    QUdpSocket serviceBroadcastReceiver;

    //State of the client in a session
    QHash<QString, int> m_currentState;

    //Number of peers who have acknowledged already
    QHash<QString, long> m_ackedPeers;

    //TODO to be removed
    QString activeSession;

    //While a user is joining, the client keeps its data in here
    QHash<QString, QVector<QByteArray> *> m_drawingBuffer;

signals:
    // external signals that are meant to be used for the user interface
    void foundCollaborationServer(QHostAddress serverAddress, QString serverUserName);
    void loginResult(bool result, QString infoMsg);
    void sessionListAvailable(QStringList newSessionList);
    void sessionJoinResult(QString sessionName, QChar result, QHash<QString, long> users);
    void drawingArrived(QString sessionName, QByteArray picData, bool initialState);

    // internal signals that will be connected to the ProtocolHandler
    void sendLoginRequest(QString destUserName);
    void sendLogoutRequest(QString destUserName);
    void sendPeerHandshake(QString destUserName, QString sessionName);
    void sendPictureRequest(QString destUserName, QString sessionName);
    void sendSessionJoinRequest(QString destUserName, QString sessionName, QString password);
    void sendSessionLeaveRequest(QString destUserName, QString sessionName);
    void sendSessionListRequest(QString destUserName);
    void sendUpdateDrawing(QString destUserName, QString sessionName, QByteArray picData);
    void sendUpdateDrawingServer(QString destUserName, QString sessionName, QByteArray picData);
    void sendWritePermissionRequest(QString destUserName);
    void sendSessionCreateRequest(QString destUserName, QString sessionName, QString password);

private slots:
    void receivedLoginResponse(QString userName, QChar result, QString infoMsg);
    void receivedPeerHandshake(QString userName, QString sessionName);
    void receivedPictureResponse(QString userName, QString sessionName, QByteArray picData);
    void receivedSessionJoinResponse(QString userName, QString sessionName, char result, unsigned int userCount, QHash<QString, long> users);
    void receivedSessionLeaveResponse(QString userName, QString sessionName, char result);
    void receivedSessionListResponse(QString userName, QStringList sessionList);
    void receivedSessionMemberUpdate(QString userName, QString sessionName, char updateType, QString user);
    void receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData);
    void receivedWritePermissionStatus(QString userName, QChar status);
    void receivedSessionCreateResponse(QString userName, QString sessionName, QChar result, QString password);

private slots:
    void gotServiceBroadcast();

};

#endif // COLLABORATIONCLIENT_H
