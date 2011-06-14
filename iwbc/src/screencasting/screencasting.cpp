/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "appglobals.h"
#include "screencasting.h"

#include <QDateTime>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

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
        m_backendProcess.start(m_commandLine);
        m_backendProcess.waitForStarted();

        if(m_backendProcess.state() == QProcess::NotRunning) {
            // we have a problem
            displayErrorMessage("Could not start specified screencasting backend");
            return;
        }

        m_isRunning = true;
    }
}

void Screencasting::processStateChanged(QProcess::ProcessState newState)
{
    // qWarning() << "backend process has a new state:" << newState;
    // TODO IMPORTANT do state change monitoring thingies here
}


void Screencasting::stopScreencasting()
{
    qint64 finalFileSize = 0;
    QString baseMsg = "A screencast of %2 %1 has been recorded. Would you like to keep this file?";
    QMessageBox msg;
    if(m_isRunning) {
        // wait three seconds before termination to make sure the latest
        // events are also recorded
        // TODO show a waiting message here
        sleep(3);
        // TODO find a better way to stop the recording
        m_backendProcess.terminate();
        QFile outputFile(m_targetFileName);
        if(!outputFile.exists()) {
            displayErrorMessage("An error occured while obtaining the recorded screencast");
            return;
        } else {
            // calculate file size in kilobytes
            finalFileSize = outputFile.size() / 1024;
            // if file is > 1024 KB convert to megabytes
            if(finalFileSize > 1024) {
                finalFileSize = finalFileSize / 1024;
                baseMsg = baseMsg.arg("MB");
            } else
                baseMsg = baseMsg.arg("KB");
            // insert the file size into the message
            baseMsg = baseMsg.arg(finalFileSize);
            // set up the messagebox
            msg.setText(baseMsg);
            msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            msg.setDefaultButton(QMessageBox::Yes);
            if(msg.exec() == QMessageBox::Yes) {
                // ask the user for the saving location
                QString path = QFileDialog::getExistingDirectory(0, "Select a directory to save screencast");
                if(path != "") {
                    QFileInfo fi(outputFile);
                    outputFile.copy(path + "/" + fi.fileName());
                    displayInfoMessage("Screencast saved to \n" + path + "/" + fi.fileName());
                    // the old copy will be removed after we fall through the if block
                }
            }
            // remove copy of file in the local dir
            outputFile.remove();
        }
    }
}


bool Screencasting::isRunning()
{
    return m_isRunning;
}
