#ifndef SCREENCASTING_H
#define SCREENCASTING_H

#include <QObject>
#include <QProcess>

class Screencasting : public QObject
{
    Q_OBJECT
public:
    explicit Screencasting(QObject *parent = 0);

    void startScreencasting();
    void stopScreencasting();

    bool isRunning();

private:
    QString m_backend;
    QString m_targetFileName;
    QString m_commandLine;
    QString m_baseCommandLine;
    QString m_container;

    QProcess m_backendProcess;
    bool m_isRunning;
    // TODO FEATURE make these additional parameters changeable
    // TODO FEATURE implement pause-and-resume support
    /*int m_fps;
    int m_caching;
    QString m_videoCodec;
    int m_videoBitrate;
    int m_videoScale;
    QString m_audioCodec;
    QString m_fileMux;*/

signals:

    void screencastingStarted();
    void screencastingStopped();

private slots:

    void processStateChanged(QProcess::ProcessState newState);

};

#endif // SCREENCASTING_H
