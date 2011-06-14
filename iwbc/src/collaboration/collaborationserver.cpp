/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#include "collaborationserver.h"

// the CollaborationServer class acts as the backbone of the p2p/server-client
// hybrid collaboration architecture. its primary responsibilities are:
// - broadcast its own IP to the local network over UDP 45455 so that clients
//   can find it
// - keep track of active clients via login/logout messages
// - keep track of collaboration sessions and their participants, store the current
//   state of the collaborative drawing

// all actual communication is meant to be handled by the ProtocolHandler class,
// via signal/slot connections to the CollaborationServer

CollaborationServer::CollaborationServer(QObject *parent) :
    QObject(parent)
{
    m_protocolHandler = NULL;
    m_serverUserName = COLLABORATION_SERVER_NAME;
    connect(&serviceBroadcastTimer, SIGNAL(timeout()), this, SLOT(serviceBroadcastTimeout()));
    serviceBroadcastTimer.start(SERVICE_BROADCAST_PERIOD_MS);

    m_sessionList.append("Test");
    m_sessionData["Test"] = new CollaborationSession();
    m_sessionData["Test"]->setSessionName("Test");
    m_sessionData["Test"]->setSessionPassword(QCryptographicHash::hash(QString("1234").toAscii(), QCryptographicHash::Md5));
}

void CollaborationServer::receivedLoginRequest(QString userName)
{
    // the specified user wants to log in
    // check if there is a user with this username already logged in
    if(m_userList.contains(userName)) {
        // user with this name already logged in
        qWarning() << userName << "requested to log in but failed, already exists";
        emit sendLoginResponse(userName, 0, "User " + userName + " already exists!");
        return;
    } else {
        // add user to list of logged in users
        m_userList.append(userName);
        qWarning() << userName << "logged in successfully";
        qWarning() << "list of inloggad users" << m_userList;
        emit sendLoginResponse(userName, 1, "Login for user " + userName + " successful!");
    }
}

void CollaborationServer::receivedLogoutRequest(QString userName)
{
    // the specified user wants to log out
    // check if there is a user with this username already logged in
    if(m_userList.contains(userName)) {
        // user with this name already logged in
        // so we can log him/her out
        // TODO remove user from existing sessions and do necessary updates
        m_userList.removeOne(userName);
        qWarning() << userName << "logged out successfully";
        return;
    } else {
        // user is not in the logged in list, can't log out
        // do nothing
        qWarning() << userName << "requested to log out but was not logged in!";
        return;
    }
}

void CollaborationServer::receivedPictureRequest(QString userName, QString sessionName)
{
    // first, check if the session with given name exists
    if(!m_sessionList.contains(sessionName)) {
        // no such session
        qWarning() << "receivedPictureRequest error: session" << sessionName << "does not exist!";
        // TODO send error reply to client
        return;
    }

    // now check if this user has joined this session
    if(!m_sessionData[sessionName]->getSessionParticipants().contains(userName)) {
        // user is not a member of this sesion
        qWarning() << "receivedPictureRequest error: user" << userName << "is not a member of" << sessionName;
        return;
    }

    // everything OK, we can send the session drawing state
    QPicture tmpPic = m_sessionData[sessionName]->getSessionDrawingState();

    qWarning() << "sending picture response" << userName << sessionName << tmpPic.size();

    emit sendPictureResponse(userName, sessionName, QByteArray::fromRawData(tmpPic.data(), tmpPic.size()));


    //TODO How do we know sending the picture has been completed?
    // send sessionMember update to the other clients in the session

    QHash<QString, long>::iterator itr;
    QHash<QString, long> clients = m_sessionData[sessionName]->getSessionParticipants();
    for (itr = clients.begin(); itr != clients.end(); itr++)
    {
        if(itr.key() != userName)
            emit sendSessionMemberUpdate(itr.key(), sessionName, UPDATE_SESSION_JOIN_END, userName);
    }
}

void CollaborationServer::receivedSessionJoinRequest(QString userName, QString sessionName, QString password)
{
    QHostAddress userAddress(m_protocolHandler->getUserMapping(userName));
    // first, check if the session with given name exists
    if(!m_sessionList.contains(sessionName)) {
        // no such session
        qWarning() << "receivedSessionJoinRequest error: session" << sessionName << "does not exist!";
        // TODO send error reply to client
        return;
    }

    if(m_sessionData[sessionName]->addSessionParticipant(userName, password, userAddress.toIPv4Address())) {
        // user successfully joined the session
        QHash<QString, long> participants = m_sessionData[sessionName]->getSessionParticipants();
        // send session member update to all other clients
        QHashIterator<QString, long> i(participants);
         while (i.hasNext()) {
             i.next();
             if(i.key() != userName)
                emit sendSessionMemberUpdate(i.key(), sessionName, UPDATE_SESSION_JOIN_BEGIN, userName);
         }
         // send join response to the newly joined client
         emit sendSessionJoinResponse(userName, sessionName, 1, participants);
         return;
     } else {
         // there was a problem with the user joining the session
         emit sendSessionJoinResponse(userName, sessionName, 0, QHash<QString, long>());
         qWarning() << "receivedSessionJoinRequest error: user" << userName << "could not join session" << sessionName;
         // TODO send error message to client
         return;
     }
}

void CollaborationServer::receivedSessionLeaveRequest(QString userName, QString sessionName)
{
    //Remove the user from the list of that session
    QHash<QString, long> *userList = &(m_sessionData[sessionName]->getSessionParticipants());
    userList->remove(userName);

    //Warn each user in the aforementioned session that a user left
    QHash<QString, long>::iterator iter;
    for (iter = userList->begin(); iter != userList->end(); iter++)
    {
        emit sendSessionMemberUpdate(iter.key(), sessionName, UPDATE_SESSION_LEAVE, userName);
    }
    //TODO Probably to be removed, no need for acknowledgement
    emit sendSessionLeaveResponse(userName, sessionName, 1);
}

void CollaborationServer::receivedSessionListRequest(QString userName)
{
    qWarning() << userName << "wants list of sessions, sending" << m_sessionList;
    emit sendSessionListResponse(userName, m_sessionList);
}

void CollaborationServer::receivedUpdateDrawingServer(QString userName, QString sessionName, QByteArray picData)
{
    qWarning() << "received update drawing" << userName << sessionName << picData.size();
    // first, check if the session with given name exists
    if(!m_sessionList.contains(sessionName)) {
        // no such session
        qWarning() << "receivedUpdateDrawingServer error: session" << sessionName << "does not exist!";
        // TODO send error reply to client
        return;
    }

    // now check if this user has joined this session
    if(!m_sessionData[sessionName]->getSessionParticipants().contains(userName)) {
        // user is not a member of this sesion
        qWarning() << "receivedUpdateDrawingServer error: user" << userName << "is not a member of" << sessionName;
        return;
    }

    // everything OK
    QPicture tmpPic;
    tmpPic.setData(picData.constData(), picData.length());
    // send data to session
    m_sessionData[sessionName]->addDrawingStep(tmpPic);
}

void CollaborationServer::receivedWritePermissionRequest(QString userName)
{
    // TODO
}

void CollaborationServer::receivedSessionCreateRequest(QString userName, QString sessionName, QString password)
{
    //Check if a session with the same name exists
    if (m_sessionList.contains(sessionName))
    {
        //The session already exists, respond negative
        emit sendSessionCreateResponse(userName, sessionName, 0, "");
        return;
    }

    //Create session with the given name
    m_sessionList.append(sessionName);
    m_sessionData[sessionName] = new CollaborationSession();
    m_sessionData[sessionName]->setSessionName(sessionName);
    m_sessionData[sessionName]->setSessionPassword(password);
    qWarning() << "The password arrived at the server : " << password;

    //Session creation is successful, send a positive response
    emit sendSessionCreateResponse(userName, sessionName, 1, password);
}


// sets the ProtocolHandler for this CollaborationServer
void CollaborationServer::setProtocolHandler(ProtocolHandler * newProtocolHandler)
{
    if(!newProtocolHandler) {
        qWarning() << "Cannot set a null protocol handler for CollaborationServer!";
        return;
    }

    if(m_protocolHandler) {
        // disconnect all signals and slots from previous protocol handler
        disconnect(this);
        disconnect(m_protocolHandler);
    }
    // connect signals and slots for new protocol handler
    // signals from server to the protocol handler slots
    connect(this, SIGNAL(sendLoginResponse(QString,char,QString)), newProtocolHandler, SLOT(sendLoginResponse(QString,char,QString)));
    connect(this, SIGNAL(sendPictureResponse(QString,QString,QByteArray)), newProtocolHandler, SLOT(sendPictureResponse(QString,QString,QByteArray)));
    connect(this, SIGNAL(sendSessionJoinResponse(QString,QString,char,QHash<QString,long>)), newProtocolHandler, SLOT(sendSessionJoinResponse(QString,QString,char,QHash<QString,long>)));
    connect(this, SIGNAL(sendSessionLeaveResponse(QString,QString,char)), newProtocolHandler, SLOT(sendSessionLeaveResponse(QString,QString,char)));
    connect(this, SIGNAL(sendSessionListResponse(QString,QStringList)), newProtocolHandler, SLOT(sendSessionListResponse(QString,QStringList)));
    connect(this, SIGNAL(sendSessionMemberUpdate(QString,QString,char,QString)), newProtocolHandler, SLOT(sendSessionMemberUpdate(QString,QString,char,QString)));
    connect(this, SIGNAL(sendWritePermissionStatus(QString,QChar)),newProtocolHandler, SLOT(sendWritePermissionStatus(QString,QChar)));
    connect(this, SIGNAL(sendSessionCreateResponse(QString,QString,QChar,QString)), newProtocolHandler, SLOT(sendSessionCreateResponse(QString,QString,QChar,QString)));

    // signals from protocol handler to server slots
    connect(newProtocolHandler, SIGNAL(receivedLoginRequest(QString)), this, SLOT(receivedLoginRequest(QString)));
    connect(newProtocolHandler, SIGNAL(receivedLogoutRequest(QString)), this, SLOT(receivedLogoutRequest(QString)));
    connect(newProtocolHandler, SIGNAL(receivedPictureRequest(QString,QString)), this, SLOT(receivedPictureRequest(QString,QString)));
    connect(newProtocolHandler, SIGNAL(receivedSessionJoinRequest(QString,QString,QString)), this, SLOT(receivedSessionJoinRequest(QString,QString,QString)));
    connect(newProtocolHandler, SIGNAL(receivedSessionLeaveRequest(QString,QString)), this, SLOT(receivedSessionLeaveRequest(QString,QString)));
    connect(newProtocolHandler, SIGNAL(receivedSessionListRequest(QString)), this, SLOT(receivedSessionListRequest(QString)));
    connect(newProtocolHandler, SIGNAL(receivedUpdateDrawingServer(QString,QString,QByteArray)), this, SLOT(receivedUpdateDrawingServer(QString,QString,QByteArray)));
    connect(newProtocolHandler, SIGNAL(receivedWritePermissionRequest(QString)), this, SLOT(receivedWritePermissionRequest(QString)));
    connect(newProtocolHandler, SIGNAL(receivedSessionCreateRequest(QString,QString,QString)), this, SLOT(receivedSessionCreateRequest(QString,QString,QString)));

    // set protocol handler user name
    newProtocolHandler->setUserName(m_serverUserName);

    m_protocolHandler = newProtocolHandler;
}

ProtocolHandler * CollaborationServer::getProtocolHandler()
{
    return m_protocolHandler;
}

// broadcast the server's address(es) over UDP at certain intervals
// so that clients in the network can auto-discover the server
void CollaborationServer::serviceBroadcastTimeout()
{
    QByteArray broadcastPackage;
    QDataStream packageStream(&broadcastPackage, QIODevice::ReadWrite);
    packageStream << QString("WTCOLSRV");
    packageStream << m_serverUserName;
    QNetworkInterface interface;
    QList<QHostAddress> IpList = interface.allAddresses();

    for (int i = 0; i < IpList.size(); i++)
        if (IpList.at(i) != QHostAddress("127.0.0.1") && IpList.at(i).protocol() == QAbstractSocket::IPv4Protocol) { // local loopback isn't useful for others
            packageStream << IpList.at(i);
        }

    serviceBroadcastSocket.writeDatagram(broadcastPackage, QHostAddress::Broadcast, SERVICE_BROADCAST_PORT);
}

void CollaborationServer::setServerUserName(QString newUserName)
{
    if(m_protocolHandler)
        m_protocolHandler->setUserName(newUserName);

    m_serverUserName = newUserName;
}

QString CollaborationServer::getServerUserName()
{
    return m_serverUserName;
}
