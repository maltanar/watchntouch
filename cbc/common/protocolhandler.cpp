#include "protocolhandler.h"

ProtocolHandler::ProtocolHandler(QObject *parent) :
    QObject(parent)
{
    m_messageTransceiver = NULL;

    // connect the loopback signal-slot pair
    connect(this, SIGNAL(sendMessageLoopback(QString,QByteArray)), this, SLOT(receiveMessage(QString,QByteArray)));
}

void ProtocolHandler::addUserMapping(QString userName, QString IP)
{
    peerMap.insert(userName, IP);
}

QString ProtocolHandler::getUserMapping(QString userName)
{
    return peerMap.value(userName, "");
}

QString ProtocolHandler::getUserName()
{
    return userName;
}

void ProtocolHandler::setUserName(QString username)
{
    this->userName = username;
}

// receiveMessage is responsible for handling byte array data coming
// from the MessageTransceiver by checking for proper message format,
// deserializing it, constructing the appropriate message object
// and passing it to its corresponding handler
void ProtocolHandler::receiveMessage(QString origin, QByteArray data)
{
    // check for zero-length message
    if(data.length() == 0) {
        // TODO what to do?
        qWarning() << "ProtocolHandler::receiveMessage got message with length" << data.length();
        return;
    }

    WTMessage candidateMsg;
    // TODO pass all byte arrays as non-pointers
    candidateMsg.deserialize(data);
    // check if deserialized data is of correct format!
    if(candidateMsg.getVersion() != "WTC1") {
        // incorrect or damaged message
        qWarning() <<  "ProtocolHandler::receiveMessage got message with incorrect header" << candidateMsg.getVersion();
        return;
    }
    qWarning() << "command:" << candidateMsg.getCommand();
    if (candidateMsg.getCommand() == "LOGINREQ")
    {
        WTLoginMessage *msg = new WTLoginMessage();
        msg->deserialize(data);
        handleLoginRequest(msg, origin);
    }
    else if (candidateMsg.getCommand() == "LOGINRES")
    {
        WTLoginResponse *msg = new WTLoginResponse();
        msg->deserialize(data);
        handleLoginResponse(msg);
    }
    else if (candidateMsg.getCommand() == "LOGOUTRQ")
    {
        WTLogoutRequest *msg = new WTLogoutRequest();
        msg->deserialize(data);
        handleLogoutRequest(msg);
    }
    else if (candidateMsg.getCommand() == "COLSTARQ")
    {
        WTPictureRequest *msg = new WTPictureRequest();
        msg->deserialize(data);
        handlePictureRequest(msg);
    }
    else if (candidateMsg.getCommand() == "COLSTARS")
    {
        WTPictureResponse *msg = new WTPictureResponse();
        msg->deserialize(data);
        handlePictureResponse(msg);
    }
    else if (candidateMsg.getCommand() == "SESSJOIN")
    {
        WTSessionJoinRequest *msg = new WTSessionJoinRequest();
        msg->deserialize(data);
        handleSessionJoinRequest(msg);
    }
    else if (candidateMsg.getCommand() == "SESSRESP")
    {
        WTSessionJoinResponse *msg = new WTSessionJoinResponse();
        msg->deserialize(data);
        handleSessionJoinResponse(msg);
    }
    else if (candidateMsg.getCommand() == "SESSQUIT")
    {
        WTSessionLeaveRequest *msg = new WTSessionLeaveRequest();
        msg->deserialize(data);
        handleSessionLeaveRequest(msg);
    }
    else if (candidateMsg.getCommand() == "SESSQACK")
    {
        WTSessionLeaveResponse *msg = new WTSessionLeaveResponse();
        msg->deserialize(data);
        handleSessionLeaveResponse(msg);
    }
    else if (candidateMsg.getCommand() == "SESSLREQ")
    {
        WTSessionListRequest *msg = new WTSessionListRequest();
        msg->deserialize(data);
        handleSessionListRequest(msg);
    }
    else if (candidateMsg.getCommand() == "SESSLRES")
    {
        WTSessionListResponse *msg = new WTSessionListResponse();
        msg->deserialize(data);
        handleSessionListResponse(msg);
    }
    else if (candidateMsg.getCommand() == "SESSMUPD")
    {
        WTSessionMemberUpdate *msg = new WTSessionMemberUpdate();
        msg->deserialize(data);
        handleSessionMemberUpdate(msg);
    }
    else if (candidateMsg.getCommand() == "DRAWUPDT")
    {
        WTUpdateDrawing *msg = new WTUpdateDrawing();
        msg->deserialize(data);
        handleUpdateDrawing(msg);
    }
    else if (candidateMsg.getCommand() == "DRAWSUPD")
    {
        WTUpdateDrawingServer *msg = new WTUpdateDrawingServer();
        msg->deserialize(data);
        handleUpdateDrawingServer(msg);
    }
    else if (candidateMsg.getCommand() == "WRTPRMRQ")
    {
        WTWritePermissionRequest *msg = new WTWritePermissionRequest();
        msg->deserialize(data);
        handleWritePermissionRequest(msg);
    }
    else if (candidateMsg.getCommand() == "WRTPRMST")
    {
        WTWritePermissionStatus *msg = new WTWritePermissionStatus();
        msg->deserialize(data);
        handleWritePermissionStatus(msg);
    }
    else if (candidateMsg.getCommand() == "HELLOPAL")
    {
        WTPeerHandshake *msg = new WTPeerHandshake();
        msg->deserialize(data);
        handlePeerHandshake(msg, origin);
    }
    else if (candidateMsg.getCommand() == "SESCRTRQ")
    {
        WTSessionCreateRequest *msg = new WTSessionCreateRequest();
        msg->deserialize(data);
        handleSessionCreateRequest(msg);
    }
    else if (candidateMsg.getCommand() == "SESCRTRS")
    {
        WTSessionCreateResponse *msg = new WTSessionCreateResponse();
        msg->deserialize(data);
        handleSessionCreateResponse(msg);
    }
}

// deliverMessage is responsible for handing the given message in byte array
// form to the MessageTransceiver with the corresponding peer destination
// for this username
bool ProtocolHandler::deliverMessage(WTMessage * msg)
{
    // check for null messages
    if(!msg)
        return false;
    // serialize the message into a byte array
    QByteArray msgData = msg->serialize();
    // check for loopback (send message to self)
    if(msg->getDestUsername() == getUserName()) {
        qWarning() << "message loopback!";
        emit sendMessageLoopback(msg->getDestUsername(), msgData);
        delete msg;
        return true;
    }
    // find the peer destination for this username
    QString destination = peerMap.value(msg->getDestUsername(), "");
    if(destination == "") {
        // the destination address for this peer is not defined
        qWarning() << "deliverMessage: destination address for peer" << msg->getDestUsername() << "undefined";
        delete msg;
        return false;
    }
    // time for delivery!
    emit sendMessage(destination, msgData);
    // we should be able to delete this message now
    // TODO handle the new - delete logic more carefully! what happens when errors
    // occur, for example?
    delete msg;
    return true;
}


void ProtocolHandler::handleMapRequestStatus(QString username, bool confirmed)
{

    if(confirmed) {
        // this mapping has been confirmed
        // we don't need the destination parameter, look for the username
        // in the pending map requests and move it to the established mappings
        QString destination = pendingMapRequests.value(username, "");
        if(destination == "") {
            // oops - something went wrong
            // no such mapping request pending
            // TODO deeper error handling?
            qWarning() << "no pending mapping request found for confirming" << username;
            return;
        } else {
            // add this mapping to the list of established peer mappings
            peerMap.insert(username, destination);
        }
    }

    // remove from list of pending requests
    pendingMapRequests.remove(username);
}


void ProtocolHandler::handleLoginRequest(WTLoginMessage *msg, QString requestOrigin)
{
    // Local peer role: Server
    // add a mapping request for the remote peer
    // this will be verified or discarded upon login result
    pendingMapRequests.insert(msg->getSrcUsername(), requestOrigin);

    emit receivedLoginRequest(msg->getSrcUsername());
}

void ProtocolHandler::handleLoginResponse(WTLoginResponse *msg)
{
    // Local peer role: Client
    emit receivedLoginResponse(msg->getSrcUsername(), msg->getResult(), msg->getInfomsg());
}

void ProtocolHandler::handleLogoutRequest(WTLogoutRequest *msg)
{
    // Local peer role: Server
    // remove mapping for this peer
    peerMap.remove(msg->getSrcUsername());
    emit receivedLogoutRequest(msg->getSrcUsername());
}

void ProtocolHandler::handlePictureRequest(WTPictureRequest *msg)
{
    //Local peer role: Server
    emit receivedPictureRequest(msg->getSrcUsername(), msg->getSessionName());
}

void ProtocolHandler::handlePictureResponse(WTPictureResponse *msg)
{
    //Local peer role: Client
    emit receivedPictureResponse(msg->getSrcUsername(), msg->getSessionName(), msg->getPicData());
}

void ProtocolHandler::handleSessionJoinRequest(WTSessionJoinRequest *msg)
{
    //Local peer role: Server
    emit receivedSessionJoinRequest(msg->getSrcUsername(), msg->getSessionName(), msg->getPassword());
}

void ProtocolHandler::handleSessionJoinResponse(WTSessionJoinResponse *msg)
{
    //Local peer role: Client
    emit receivedSessionJoinResponse(msg->getSrcUsername(), msg->getSessionName(), msg->getResult(), msg->getUserCount(), msg->getUsers());
}

void ProtocolHandler::handleSessionLeaveRequest(WTSessionLeaveRequest *msg)
{
    //Local peer role: Server
    emit receivedSessionLeaveRequest(msg->getSrcUsername(), msg->getSessionName());
}

void ProtocolHandler::handleSessionLeaveResponse(WTSessionLeaveResponse *msg)
{
    //Local peer role: Client
    emit receivedSessionLeaveResponse(msg->getSrcUsername(), msg->getSessionName(), msg->getResult());
}

void ProtocolHandler::handleSessionListRequest(WTSessionListRequest *msg)
{
    //Local peer role: Server
    emit receivedSessionListRequest(msg->getSrcUsername());
}

void ProtocolHandler::handleSessionListResponse(WTSessionListResponse *msg)
{
    //Local peer role: Client
    emit receivedSessionListResponse(msg->getSrcUsername(), msg->getSessionList());
}

void ProtocolHandler::handleSessionMemberUpdate(WTSessionMemberUpdate *msg)
{
    //Local peer role: Server
    emit receivedSessionMemberUpdate(msg->getSrcUsername(), msg->getSessionName(), msg->getUpdateType(), msg->getUser());
}

void ProtocolHandler::handleUpdateDrawing(WTUpdateDrawing *msg)
{
    //Local peer role: Client
    emit receivedUpdateDrawing(msg->getSrcUsername(), msg->getSessionName(), msg->getPicData());
}

void ProtocolHandler::handleUpdateDrawingServer(WTUpdateDrawingServer *msg)
{
    //Local peer role: Server
    emit receivedUpdateDrawingServer(msg->getSrcUsername(), msg->getSessionName(), msg->getPicData());
}

void ProtocolHandler::handleWritePermissionRequest(WTWritePermissionRequest *msg)
{
    //Local peer role: Server
    emit receivedWritePermissionRequest(msg->getSrcUsername());
}

void ProtocolHandler::handleWritePermissionStatus(WTWritePermissionStatus *msg)
{
    //Local peer role: Client
    emit receivedWritePermissionStatus(msg->getSrcUsername(), msg->getStatus());
}

void ProtocolHandler::handlePeerHandshake(WTPeerHandshake *msg, QString requestOrigin)
{
    //Local peer role: Client
    peerMap.insert(msg->getSrcUsername(), requestOrigin);
    emit receivedPeerHandshake(msg->getSrcUsername(), msg->getSessionName());
}

void ProtocolHandler::handleSessionCreateRequest(WTSessionCreateRequest *msg)
{
    //Local peer role: Server
    emit receivedSessionCreateRequest(msg->getSrcUsername(),msg->getSessionName(), msg->getPassword());
}

void ProtocolHandler::handleSessionCreateResponse(WTSessionCreateResponse *msg)
{
    //Local peer role: Client
    emit receivedSessionCreateResponse(msg->getSrcUsername(), msg->getSessionName(), msg->getResult(), msg->getPassword());
}


void ProtocolHandler::sendLoginRequest(QString destUserName)
{
    WTLoginMessage *msg = new WTLoginMessage;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    deliverMessage(msg);
}

void ProtocolHandler::sendLoginResponse(QString destUserName, char result, QString infoMsg)
{
    // Local peer role: server
    // if login was OK, add pending mapping
    // TODO encode this '1' in a more meaningful way
    handleMapRequestStatus(destUserName, (bool) result);
    WTLoginResponse *msg = new WTLoginResponse;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setResult(result);
    msg->setInfomsg(infoMsg);
    deliverMessage(msg);
}

void ProtocolHandler::sendLogoutRequest(QString destUserName)
{
    WTLogoutRequest *msg = new WTLogoutRequest;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    deliverMessage(msg);
}

void ProtocolHandler::sendPeerHandshake(QString destUserName, QString sessionName)
{
    WTPeerHandshake *msg = new WTPeerHandshake;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    deliverMessage(msg);
}

void ProtocolHandler::sendPictureRequest(QString destUserName, QString sessionName)
{
    WTPictureRequest *msg = new WTPictureRequest;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    deliverMessage(msg);
}

void ProtocolHandler::sendPictureResponse(QString destUserName, QString sessionName, QByteArray picData)
{
    WTPictureResponse *msg = new WTPictureResponse;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setPicData(picData);
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionJoinRequest(QString destUserName, QString sessionName, QString password)
{
    WTSessionJoinRequest *msg = new WTSessionJoinRequest;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setPassword(password);
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionJoinResponse(QString destUserName, QString sessionName, char result, QHash<QString, long> users)
{
    WTSessionJoinResponse *msg = new WTSessionJoinResponse;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setResult(result);
    msg->setUsers(users);
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionLeaveRequest(QString destUserName, QString sessionName)
{
    WTSessionLeaveRequest *msg = new WTSessionLeaveRequest;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionLeaveResponse(QString destUserName, QString sessionName, char result)
{
    WTSessionLeaveResponse *msg = new WTSessionLeaveResponse;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setResult(result);
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionListRequest(QString destUserName)
{
    WTSessionListRequest *msg = new WTSessionListRequest;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionListResponse(QString destUserName, QStringList sessionList)
{
    WTSessionListResponse *msg = new WTSessionListResponse;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionList(sessionList);
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionMemberUpdate(QString destUserName, QString sessionName, char updateType, QString user)
{
    WTSessionMemberUpdate *msg = new WTSessionMemberUpdate;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setUpdateType(updateType);
    msg->setUser(user);
    deliverMessage(msg);
}

void ProtocolHandler::sendUpdateDrawing(QString destUserName, QString sessionName, QByteArray picData)
{
    WTUpdateDrawing *msg = new WTUpdateDrawing;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setPicData(picData);
    deliverMessage(msg);
}

void ProtocolHandler::sendUpdateDrawingServer(QString destUserName, QString sessionName, QByteArray picData)
{
    WTUpdateDrawingServer *msg = new WTUpdateDrawingServer;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setPicData(picData);
    deliverMessage(msg);
}

void ProtocolHandler::sendWritePermissionRequest(QString destUserName)
{
    WTWritePermissionRequest *msg = new WTWritePermissionRequest;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    deliverMessage(msg);
}

void ProtocolHandler::sendWritePermissionStatus(QString destUserName, QChar status)
{
    WTWritePermissionStatus *msg = new WTWritePermissionStatus;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setStatus(status.toAscii());
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionCreateRequest(QString destUserName, QString sessionName, QString password)
{
    WTSessionCreateRequest *msg = new WTSessionCreateRequest;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setPassword(password);
    deliverMessage(msg);
}

void ProtocolHandler::sendSessionCreateResponse(QString destUserName, QString sessionName, QChar result, QString password)
{
    WTSessionCreateResponse *msg = new WTSessionCreateResponse;
    msg->setSrcUsername(this->userName);
    msg->setDestUsername(destUserName);
    msg->setSessionName(sessionName);
    msg->setResult(result);
    msg->setPassword(password);
    deliverMessage(msg);
}


void ProtocolHandler::setMessageTransceiver(MessageTransceiver * newMesssageTransceiver)
{
    if(m_messageTransceiver) {
        // disconnect all signals and slots from previous MessageTransceiver
        disconnect(this);
        disconnect(m_messageTransceiver);
    }
    // connect signals and slots
    connect(this, SIGNAL(sendMessage(QString,QByteArray)), newMesssageTransceiver, SLOT(sendMessage(QString,QByteArray)));
    connect(newMesssageTransceiver, SIGNAL(gotNewData(QString,QByteArray)), this, SLOT(receiveMessage(QString,QByteArray)));
}

MessageTransceiver* ProtocolHandler::getMessageTransceiver()
{
    return m_messageTransceiver;
}
