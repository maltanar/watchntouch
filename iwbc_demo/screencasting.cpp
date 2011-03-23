#include "appglobals.h"
#include "screencasting.h"

#include <QDateTime>
#include <QMessageBox>

Screencasting::Screencasting(QObject *parent) :
    QObject(parent)
{
    m_backend = "vlc";
    m_container = "mp4";
    m_baseCommandLine = "vlc screen:// -I dummy --screen-follow-mouse --no-video :screen-fps=5 :screen-caching=300 --sout \"#transcode{vcodec=h264,vb=400,fps=5,scale=0,acodec=none}:duplicate{dst=std{access=file,mux=mp4,dst='{TARGET_FILE}'}}\"";
    m_targetFileName = "screencast - " + QDateTime::currentDateTime().toString()+"." +m_container;
    m_isRunning = false;

    connect(&m_backendProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(processStateChanged(QProcess::ProcessState)));
}

// TODO check if the specified backend is installed

void Screencasting::startScreencasting()
{
    if(!m_isRunning) {
        m_commandLine = m_baseCommandLine.replace("{TARGET_FILE}", m_targetFileName);
        qWarning() << "starting" << m_commandLine;
        // TODO show info about starting screencast
        m_backendProcess.start(m_commandLine);
        m_backendProcess.waitForStarted();

        if(m_backendProcess.state() == QProcess::NotRunning) {
            // we have a problem
            // TODO log error
            return;
        }

        m_isRunning = true;
    }
}

void Screencasting::processStateChanged(QProcess::ProcessState newState)
{
    qWarning() << "backend process has a new state:" << newState;
    // TODO IMPORTANT do state change monitoring thingies here
}


void Screencasting::stopScreencasting()
{
    if(m_isRunning) {
        // wait three seconds before termination to make sure the latest
        // events are also recorded
        // TODO show a waiting message here
        sleep(3);
        // TODO find a better way to stop the recording
        m_backendProcess.terminate();
        // TODO IMPORTANT report status and ask if user wants to save the output
    }
}


bool Screencasting::isRunning()
{
    return m_isRunning;
}
