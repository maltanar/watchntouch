/*
 * Copyright (c) 2009 Ludovic Fauvet
 *
 * Author: Ludovic Fauvet <etix@l0cal.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VIDEOUNDERLAY_H
#define VIDEOUNDERLAY_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QErrorMessage>
#include <QFileDialog>
#include <QMutex>

// libvlc includes
#include <vlc/vlc.h>
#include <vlc/libvlc.h>

#include "contentdisplay.h"

#define VIDEO_WIDTH     640
#define VIDEO_HEIGHT    480

// TODO add proper resizing for video
// TODO add play pause stop functionality
// TODO add timeline-related functionality (seeking & timeline reporting)

class VideoUnderlay : public ContentDisplay
{
    Q_OBJECT

public:
    VideoUnderlay( QWidget* parent = 0 );

    float getMediaLength();
    float getMediaPosition();
    QSize getMediaSize();

    bool selectContent(QString location);
    QString getContentContext();

public slots:
    void play();
    bool loadMedia(QString videoFileName);
    void pause();
    void stop();

private slots:
    void processNewFrame( struct ctx* );

private:
    void                    initVLC();
    bool                    catchException();
    void                    unloadVideo();

    // GUI
    QLabel*                 video;
    QVBoxLayout*            layout;
    QErrorMessage*          errorHandler;

    QString                 mrl;

    // VLC stuff
    static void             lock( struct ctx*, void** );
    static void             unlock( struct ctx* );
    libvlc_exception_t      m_ex;
    libvlc_instance_t*      m_vlcInstance;
    libvlc_media_t*         m_vlcMedia;
    libvlc_media_player_t*  m_vlcMediaplayer;

signals:
    void                    frameReady( struct ctx* );
    void timelineUpdate(float pos);
};

struct ctx
{
    uchar*                  pixels;
    QMutex*                 mutex;
    VideoUnderlay*          videoUnderlay;
};

#endif // VIDEOUNDERLAY_H
