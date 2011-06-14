/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#include "collaborationsession.h"
#include <QPixmap>
#include <QDebug>

CollaborationSession::CollaborationSession(QObject *parent) :
    QObject(parent)
{
}

void CollaborationSession::setSessionName(QString newName)
{
    m_sessionName = newName;
}

QString CollaborationSession::getSessionName()
{
    return m_sessionName;
}

void CollaborationSession::setSessionPassword(QString newPassword)
{
    m_sessionPassword = newPassword;
}

QString CollaborationSession::getSessionPassword()
{
    return m_sessionPassword;
}

QHash<QString, long> CollaborationSession::getSessionParticipants()
{
    return m_sessionParticipants;
}

void CollaborationSession::setSessionParticipants(QHash<QString, long> participantList)
{
    m_sessionParticipants = participantList;

    //Initialize all session members as unacknowledged
    QHash<QString, long>::iterator itr;
    for (itr = participantList.begin(); itr != participantList.end(); itr++)
    {
        m_sessionAcknowledgements.insert(itr.key(),false);
    }
}

bool CollaborationSession::addSessionParticipant(QString userName, QString sessionPassword, long userIpAddress)
{
    if(sessionPassword == m_sessionPassword) {
        // password is correct, check if participant already exists in this session
        if(m_sessionParticipants.contains(userName))
            //participant already exists in session
            return false;
        else {
            // everything in order, we can add this participant to the session
            m_sessionParticipants[userName] = userIpAddress;

            return true;
        }
    } else
        qWarning() << "expected md5 password = " << m_sessionPassword << " found : " << sessionPassword;
        // incorrect password
        return false;
}

void CollaborationSession::removeSessionParticipant(QString userName)
{
    if(m_sessionParticipants.contains(userName))
        m_sessionParticipants.remove(userName);
}

QPicture CollaborationSession::getSessionDrawingState()
{
    return m_sessionDrawingData;
}

void CollaborationSession::addDrawingStep(QPicture newDrawingStep)
{
    QPicture tmpPicture;
    m_picturePainter.begin(&tmpPicture);
    m_picturePainter.drawPicture(0,0,m_sessionDrawingData);
    m_picturePainter.drawPicture(0,0,newDrawingStep);
    m_picturePainter.end();

    m_sessionDrawingData = tmpPicture;

    //TODO Remove this
    QPixmap hede(800,480);
    hede.fill(Qt::white);
    m_picturePainter.begin(&hede);
    m_picturePainter.drawPicture(0,0, m_sessionDrawingData);
    m_picturePainter.end();
    hede.save("state.png");

}


void CollaborationSession::acknowledgePeer(QString userName)
{
    m_sessionAcknowledgements[userName] = true;
}

bool CollaborationSession::isAcknowledged(QString userName)
{
    return m_sessionAcknowledgements[userName];
}

bool CollaborationSession::isAllAcknowledged()
{
    QHash<QString, bool>::iterator itr;
    for (itr = m_sessionAcknowledgements.begin(); itr != m_sessionAcknowledgements.end(); itr++)
    {
        if (!itr.value()) return false;
    }
    return true;
}
