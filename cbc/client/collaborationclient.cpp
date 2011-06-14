/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#include "collaborationclient.h"

CollaborationClient::CollaborationClient(QObject *parent) :
    QObject(parent),
    m_protocolHandler(NULL)
{

    // set up the broadcast listener to autodiscover the server
    serviceBroadcastReceiver.bind(SERVICE_BROADCAST_PORT, QUdpSocket::ShareAddress);
    connect(&serviceBroadcastReceiver, SIGNAL(readyRead()), this, SLOT(gotServiceBroadcast()));
}

void CollaborationClient::setProtocolHandler(ProtocolHandler * newProtocolHandler)
{
    if (m_protocolHandler)
    {
        disconnect(this);
        disconnect(m_protocolHandler);
    }

    connect(this, SIGNAL(sendLoginRequest(QString)), newProtocolHandler, SLOT(sendLoginRequest(QString)));
    connect(this, SIGNAL(sendLogoutRequest(QString)), newProtocolHandler, SLOT(sendLogoutRequest(QString)));
    connect(this, SIGNAL(sendPeerHandshake(QString,QString)), newProtocolHandler, SLOT(sendPeerHandshake(QString,QString)));
    connect(this, SIGNAL(sendPictureRequest(QString,QString)), newProtocolHandler, SLOT(sendPictureRequest(QString,QString)));
    connect(this, SIGNAL(sendSessionJoinRequest(QString,QString,QString)), newProtocolHandler, SLOT(sendSessionJoinRequest(QString,QString,QString)));
    connect(this, SIGNAL(sendSessionLeaveRequest(QString,QString)), newProtocolHandler, SLOT(sendSessionLeaveRequest(QString,QString)));
    connect(this, SIGNAL(sendSessionListRequest(QString)), newProtocolHandler, SLOT(sendSessionListRequest(QString)));
    connect(this, SIGNAL(sendUpdateDrawing(QString,QString,QByteArray)), newProtocolHandler, SLOT(sendUpdateDrawing(QString,QString,QByteArray)));
    connect(this, SIGNAL(sendUpdateDrawingServer(QString,QString,QByteArray)), newProtocolHandler, SLOT(sendUpdateDrawingServer(QString,QString,QByteArray)));
    connect(this, SIGNAL(sendWritePermissionRequest(QString)), newProtocolHandler, SLOT(sendWritePermissionRequest(QString)));
    connect(this, SIGNAL(sendSessionCreateRequest(QString,QString,QString)), newProtocolHandler, SLOT(sendSessionCreateRequest(QString,QString,QString)));

    connect(newProtocolHandler, SIGNAL(receivedLoginResponse(QString,QChar,QString)), this, SLOT(receivedLoginResponse(QString,QChar,QString)));
    connect(newProtocolHandler, SIGNAL(receivedPeerHandshake(QString,QString)), this, SLOT(receivedPeerHandshake(QString,QString)));
    connect(newProtocolHandler, SIGNAL(receivedPictureResponse(QString,QString,QByteArray)), this, SLOT(receivedPictureResponse(QString,QString,QByteArray)));
    connect(newProtocolHandler, SIGNAL(receivedSessionJoinResponse(QString,QString,char,uint,QHash<QString,long>)), this, SLOT(receivedSessionJoinResponse(QString,QString,char,uint,QHash<QString,long>)));
    connect(newProtocolHandler, SIGNAL(receivedSessionLeaveResponse(QString,QString,char)), this, SLOT(receivedSessionLeaveResponse(QString,QString,char)));
    connect(newProtocolHandler, SIGNAL(receivedSessionListResponse(QString,QStringList)), this, SLOT(receivedSessionListResponse(QString,QStringList)));
    connect(newProtocolHandler, SIGNAL(receivedSessionMemberUpdate(QString,QString,char,QString)), this, SLOT(receivedSessionMemberUpdate(QString,QString,char,QString)));
    connect(newProtocolHandler, SIGNAL(receivedUpdateDrawing(QString,QString,QByteArray)), this, SLOT(receivedUpdateDrawing(QString,QString,QByteArray)));
    connect(newProtocolHandler, SIGNAL(receivedWritePermissionStatus(QString,QChar)), this, SLOT(receivedWritePermissionStatus(QString,QChar)));
    connect(newProtocolHandler, SIGNAL(receivedSessionCreateResponse(QString,QString,QChar,QString)), this, SLOT(receivedSessionCreateResponse(QString,QString,QChar,QString)));
    m_protocolHandler = newProtocolHandler;
}

ProtocolHandler * CollaborationClient::getProtocolHandler()
{
    return m_protocolHandler;
}


void CollaborationClient::receivedLoginResponse(QString userName, QChar result, QString infoMsg)
{
    //TODO show the infoMsg
    qWarning() << "Login Response message: " <<  infoMsg;

    if (result == 0)
    {
        //TODO show error message
        qWarning() << "Login is unsuccessful";
        emit loginResult(false, infoMsg);
    }
    else
    {
        qWarning() << "Login is successful";
        emit loginResult(true, infoMsg);
        emit sendSessionListRequest(m_serverName);
    }
}

void CollaborationClient::receivedPeerHandshake(QString userName, QString sessionName)
{
    //Connect to the user that has just said hello.

    //If this client is the one which joins to a session
    // - it is expecting to get peerHandshake messages from all the session members
    if (m_currentState[sessionName] == JOIN_SESSION_STATE)
    {
        qWarning() << "Peer " << userName << " is acknowledged";
        m_collaborationSessions[sessionName]->acknowledgePeer(userName);
        //if all members shaked hands, then request picture from server
        if (m_collaborationSessions[sessionName]->isAllAcknowledged())
        {
            qWarning() << "All peers are acknowledged.";
            m_currentState[sessionName] = JOIN_SESSION_PEERHANDHAKE_COMPLETED;
            emit sendPictureRequest(m_serverName,sessionName);
        }
    }
    else if (m_currentState[sessionName] == MEMBER_UPDATE_JOIN_BEGIN_RECEIVED)
    {
        qWarning() << "Got handshake from new peer : " << userName;
        //Add user to the session member list for this client
        m_collaborationSessions[sessionName]->addSessionParticipant(userName, m_collaborationSessions[sessionName]->getSessionPassword(), 0);
        //Shake hands with the client
        emit sendPeerHandshake(userName, sessionName);
    }
    else
    {
        qWarning() << "Received unexpected peer handshake, adding to pending list";
        m_pendingHandshakes.insert(userName, sessionName);
    }
}

void CollaborationClient::receivedPictureResponse(QString userName, QString sessionName, QByteArray picData)
{
    //Show the pictureData on screen
    QPicture sessState;
    sessState.setData(picData.constData(), picData.size());
    //Set the current picture data of the session for this client
    m_collaborationSessions[sessionName]->addDrawingStep(sessState);
    //TODO this will be emitted in collaboration session
    emit drawingArrived(sessionName, picData, true);
    //Joining to the session has been completed.
    m_currentState[sessionName] = JOIN_SESSION_COMPLETED;
    emit sessionJoinResult(sessionName, 1, m_collaborationSessions[sessionName]->getSessionParticipants());
}

void CollaborationClient::receivedSessionJoinResponse(QString userName, QString sessionName, char result, unsigned int userCount, QHash<QString, long> users)
{
    if (result == 0)
    {
        //Session join request was unsuccessful
        //TODO show error message.
        qWarning() << "Session Join by " << userName << " was unsuccessful.";
        emit sessionJoinResult(sessionName, result, users);
    }
    else
    {
        //Session join was successful.
        qWarning() << "Session Join by " << userName << " was successful";

        //TODO to be removed!
        this->activeSession = sessionName;

        //Remove the user from the list of users not to send data to the user itself
        users.remove(m_protocolHandler->getUserName());
        m_currentState.insert(sessionName, JOIN_SESSION_STATE);
        m_drawingBuffer.insert(sessionName, new QVector<QByteArray>);

        //Create a new session
        CollaborationSession *collaborationSession = new CollaborationSession;
        collaborationSession->setSessionName(sessionName);

        collaborationSession->setSessionParticipants(users);

        //Map it with its sessionName
        m_collaborationSessions.insert(sessionName, collaborationSession);

        // the sole user in the session is this user, so no handshakes needed
        // we can directly send the current state request to the server
        if (users.size() == 0)
        {
            emit sendPictureRequest(m_serverName, sessionName);
            return;
        }

        //Add all members to the list that is going to which
        // - this client will establish TCP connections
        QHash<QString, long>::iterator itr;
        for (itr = users.begin(); itr != users.end(); itr++)
        {
            //Send handshake messages to the users in the session
            if (itr.key() != m_protocolHandler->getUserName()) {
                // check if this peer is the server user, we already have a connection to that
                if(itr.key() != m_serverName)
                    m_protocolHandler->addUserMapping(itr.key(), QHostAddress(itr.value()).toString());
                qWarning() << "Peer handshake has been sent to : " << itr.key();
                emit sendPeerHandshake(itr.key(), sessionName);
            }
        }
    }
}

void CollaborationClient::receivedSessionLeaveResponse(QString userName, QString sessionName, char result)
{
    if (result == 0)
    {
        qWarning() << "Session Leave by " << userName << " was unsuccesful";
    }
    else
    {
        qWarning() << "Session Leave by " << userName << " was succesful";
        //Remove the participants from the client's collaboration session
        m_collaborationSessions[sessionName]->getSessionParticipants().clear();
        //Remove the name of the session as the client has left it
        m_collaborationSessions[sessionName]->setSessionName("");
        //Remove the password of the session as the client has left the session
        m_collaborationSessions[sessionName]->setSessionPassword("");
    }
}

void CollaborationClient::receivedSessionListResponse(QString userName, QStringList sessionList)
{
    //List the sessions so that a client can decide which one to join
    int size = sessionList.size();
    qWarning() << "Current sessions:\n";

    //Check if the client has already a session list
    // - if yes, clear it and get the new list
    if (!m_sessionList.empty())
         m_sessionList.clear();

    m_sessionList.append(sessionList);

    //Print the session list to stdout
    for (int i = 0; i < size; i++)
    {
       qWarning() << i << sessionList.at(i) << "\n";   
    }

    emit sessionListAvailable(sessionList);
}

void CollaborationClient::receivedSessionMemberUpdate(QString userName, QString sessionName, char updateType, QString user)
{

    //The users in the list "users" have started to join to the session
    if (updateType == UPDATE_SESSION_JOIN_BEGIN)
    {
        qWarning() << "New member wants to join : " << user;
        m_currentState[sessionName] = MEMBER_UPDATE_JOIN_BEGIN_RECEIVED;
        //The client here finishes what it is sending and stops sending
        // - All what it draws is stored in a buffer
    }
    //The users in the list "users" have completely joined the session
    else if (updateType == UPDATE_SESSION_JOIN_END)
    {
        m_currentState[sessionName] = MEMBER_UPDATE_JOIN_END_RECEIVED;
        //Client resumes sending as the new participant is ready
        sendBufferedData(sessionName, user);

    }
    //The users in the list "users" have left the session
    else if (updateType == UPDATE_SESSION_LEAVE)
    {
        //Delete the user from the list of participants of the session
        QHash<QString, long> *participants = &(m_collaborationSessions[sessionName]->getSessionParticipants());
        QHash<QString, long>::iterator iter;
        for (iter = participants->begin(); iter != participants->end(); iter++)
        {
            if (iter.key() == user)
            {
                participants->erase(iter);
                break;
            }
        }
    }
}

void CollaborationClient::receivedUpdateDrawing(QString userName, QString sessionName, QByteArray picData)
{
    //Update the state with the picData
    QPicture drawingStep;
    drawingStep.setData(picData.constData(), picData.size());
    m_collaborationSessions[sessionName]->addDrawingStep(drawingStep);

    qWarning() << "Picdata geldi of size : " << picData.size();

    //TODO this will be emitted in collaboration session
    emit drawingArrived(sessionName, picData, false);
}

void CollaborationClient::receivedWritePermissionStatus(QString userName, QChar status)
{
    //TODO if status 0
    //TODO - cannot draw to classroom session
    //TODO else
    //TODO - can draw to classroom session
    //TODO so, change a state variable to show the permission accordingly
}

void CollaborationClient::receivedSessionCreateResponse(QString userName, QString sessionName, QChar result, QString password)
{
    //Check if the operation was successful
    if (result == 0)
    {
        //Operation was unsuccessful
        qWarning() << "Creation of session with session name : " << sessionName << " failed";
        return;
    }

    qWarning() << "Session " << sessionName << " has been created";

    //As the creator of the session is assumed to automatically join to that session
    // it sends automatically session join request message
    qWarning() << "Join with the password : " <<  password;
    emit sendSessionJoinRequest(m_serverName, sessionName, password);
}


void CollaborationClient::createSession(QString sessionName, QString password)
{
    //Send session create message with encrypted password
    emit sendSessionCreateRequest(m_serverName, sessionName, password);
}

void CollaborationClient::gotServiceBroadcast()
{
    // receive collaborationserver service broadcasts over UDP
    // note that this is just for autodiscovery, we don't automatically connect to any
    // of the discovered servers

    QByteArray broadcastPackage;
    QString packageHeader;
    QString serverUserName;
    QHostAddress serverAddress;
    QNetworkInterface networkInterface;
    QList<QNetworkInterface> allInterfaces = networkInterface.allInterfaces();
    QList<QNetworkAddressEntry> allAddresses;

    for (int i = 0; i < allInterfaces.size(); i++)
    {
        allAddresses.append(allInterfaces[i].addressEntries());
    }

    while(serviceBroadcastReceiver.hasPendingDatagrams())
    {
        broadcastPackage.resize(serviceBroadcastReceiver.pendingDatagramSize());
        serviceBroadcastReceiver.readDatagram(broadcastPackage.data(), broadcastPackage.size());
        QDataStream packageStream(&broadcastPackage, QIODevice::ReadWrite);
        packageStream >> packageHeader;

        if (packageHeader == "WTCOLSRV")
        {
            packageStream >> serverUserName;
            // TODO we are using only the first entry of the multiple server addresses
            // modify this part to make use of all existing entries
            packageStream >> serverAddress;

            for(int i = 0; i < allAddresses.size(); i++)
            {
                if (allAddresses[i].ip().protocol() != QAbstractSocket::IPv4Protocol)
                    continue;

                if (((allAddresses[i].ip().toIPv4Address() & allAddresses[i].netmask().toIPv4Address()) ==
                     (serverAddress.toIPv4Address() & allAddresses[i].netmask().toIPv4Address())))
                {
                    //qWarning() << allAddresses[i].ip() << allAddresses[i].netmask();
                    //qWarning() << serverAddress << "!";
                    emit foundCollaborationServer(serverAddress, serverUserName);
                    break;
                }
            }
        }
    }
}


void CollaborationClient::loginToServer(QHostAddress serverAddress, QString serverName, QString userName)
{
    // TODO check if mapping for server already exists
    m_protocolHandler->addUserMapping(serverName, serverAddress.toString());
    m_serverName = serverName;
    // set the origin (source) user name
    m_protocolHandler->setUserName(userName);
    // emit login request
    emit sendLoginRequest(m_serverName);
}

void CollaborationClient::refreshSessionList()
{
    emit sendSessionListRequest(m_serverName);
}

void CollaborationClient::joinSession(QString sessionName, QString password)
{
    emit sendSessionJoinRequest(m_serverName, sessionName, QCryptographicHash::hash(password.toAscii(), QCryptographicHash::Md5));
}

void CollaborationClient::sendDrawing(QString sessionName, QByteArray picData)
{

    //Send the picData to all the peers in the session
    //Check if there is a member joining, if so, store what is drawn in a buffer
    //Here it is checked if the buffer is empty, as if it is not, there are still some data being sent
    // - and do not send before it finishes, in order not to mix the order up. Instead, store it in the
    // - buffer too
    if (m_currentState[sessionName] != MEMBER_UPDATE_JOIN_BEGIN_RECEIVED && m_drawingBuffer[sessionName]->isEmpty())
    {
        QHash<QString, long>::iterator itr;
        QHash<QString, long> *participants = &(m_collaborationSessions[sessionName]->getSessionParticipants());

        for (itr = participants->begin(); itr != participants->end(); itr++)
        {
            //Don't send the picData to the drawer itself
            if (itr.key() == m_protocolHandler->getUserName()) continue;

            qWarning() << "sent to the user : " << itr.key();
            emit sendUpdateDrawing(itr.key(), sessionName, picData);
        }
        //Send picData to the server
        emit sendUpdateDrawingServer(m_serverName, sessionName, picData);
    }
    else
    {
        //Store the picData in order to send it later
        m_drawingBuffer[sessionName]->push_back(picData);
    }
}

void CollaborationClient::leaveSession(QString sessionName)
{
    emit sendSessionLeaveRequest(m_serverName, sessionName);
}

QString CollaborationClient::getActiveSession()
{
    return this->activeSession;
}

void CollaborationClient::sendBufferedData(QString sessionName, QString user)
{
    QHash<QString, long>::iterator itr;
    QHash<QString, long> *participants = &(m_collaborationSessions[sessionName]->getSessionParticipants());

    QVector<QByteArray> *drawingBuffer = m_drawingBuffer[sessionName];

    //Send all the drawing data up to now to the users
    QVector<QByteArray>::iterator iterBuffer = drawingBuffer->begin();
    while(iterBuffer != drawingBuffer->end())
    {
        for (itr = participants->begin(); itr != participants->end(); itr++)
        {
            //Don't send the picData to the drawer itself
            if (itr.key() == m_protocolHandler->getUserName()) continue;

            qWarning() << "sent to the user : " << itr.key();
            emit sendUpdateDrawing(itr.key(), sessionName, *iterBuffer);
        }
        //Send picData to the server
        emit sendUpdateDrawingServer(m_serverName, sessionName, *iterBuffer);
        drawingBuffer->erase(iterBuffer);
    }

    //If a user was joining, let it know that you finished sending finally.
    if (m_currentState[sessionName] == MEMBER_UPDATE_JOIN_BEGIN_RECEIVED)
    {
        // check if we already received a handshake for this user and session
        if(m_pendingHandshakes.contains(user) && m_pendingHandshakes.value(user, "") == sessionName) {
            // acknowledge peer
            receivedPeerHandshake(user, sessionName);
            // clear pending ack
            m_pendingHandshakes.remove(user);
        }
    }
}

