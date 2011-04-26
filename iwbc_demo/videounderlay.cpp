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

#include <QDebug>
#include "videounderlay.h"
#include "recentlyused.h"
// TODO release allocated VLC resources in destructor

VideoUnderlay::VideoUnderlay( QWidget* parent )
        : ContentDisplay( parent )
{
    m_prevTimelineContext = -1;

    // We do not support video resizing
    setFixedSize( 640, 480 );

    // This is the widget where the video will be drawn
    // The advantage of a QLabel over a QWidget is that
    // we can easily push a QPixmap into.
    video = new QLabel( this );
    video->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // The container will contain the main layout
    // Video + Play button
    //setCentralWidget( container );

    // Vertical Layout
    layout = new QVBoxLayout;
    layout->addWidget( video );
    this->setLayout( layout );

    // Used for printing libvlc exceptions (if any)
    errorHandler = new QErrorMessage( this );

    // Forget that for the moment ;-)
    connect( this, SIGNAL( frameReady( struct ctx* ) ),
             this, SLOT( processNewFrame( struct ctx* ) ) );

    // VLC-related vars have not been constructed yet
    m_vlcMedia = NULL;
    m_vlcInstance = NULL;
    m_vlcMediaplayer = NULL;


    // Finally init libvlc
    initVLC();
}

QSize VideoUnderlay::getMediaSize()
{
    // TODO retrieve real media size from VLC
    // TODO resizing logic for too big videos
    QSize size = video->size();
    return size;
}

float VideoUnderlay::getMediaLength()
{
    float length = 0.0f;

    if(m_vlcMedia) {
        length = libvlc_media_player_get_length(m_vlcMediaplayer, &m_ex);
        catchException();
    }

    return length;
}

float VideoUnderlay::getMediaPosition()
{
    float pos = 0.0f;

    if(m_vlcMedia) {
        pos = libvlc_media_player_get_time(m_vlcMediaplayer, &m_ex);
        catchException();
    }

    return pos;
}

void VideoUnderlay::initVLC()
{
    // List of parameters used to initialize libvlc.
    // These arguments are same as those you can pass
    // the the VLC command line.
    char const* vlc_argv[] =
    {
        "--verbose", "1",
        // Edit this line if libvlc can't locate your plugins directory
        //"--plugin-path", "/path/to/vlc",
    };
    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

    // Initialize the libvlc exception mechanism
    libvlc_exception_init( &m_ex );
    // Create a libvlc instance
    m_vlcInstance = libvlc_new( vlc_argc, vlc_argv, &m_ex );
    // This for catching and printing exceptions
    // raised by libvlc
    catchException();
    // Create the mediaplayer used to play a media
    m_vlcMediaplayer = libvlc_media_player_new( m_vlcInstance, &m_ex );
    // Re-checking for exceptions
    catchException();

    // We're done with the initialization!
}

void VideoUnderlay::unloadVideo()
{
    if(m_vlcMedia) {
        pause();
        libvlc_media_release(m_vlcMedia);
        catchException();
        m_vlcMedia = NULL;
        mrl = "";
    }
}

// TODO get first frame on load
bool VideoUnderlay::loadMedia(QString videoFileName)
{
    // unload previously loaded video if it exists
    unloadVideo();

    // set the internal file name variable
    mrl = videoFileName;

    // Create a new media from the Media Resource Locator
    m_vlcMedia = libvlc_media_new( m_vlcInstance, mrl.toAscii(), &m_ex );
    // Catching exception
    catchException();

    // We now need a struct for storing the video buffer
    // and a mutex to protect it.
    // The structure will be given as an arguments for the
    // lock/unlock callbacks.
    struct ctx* context;
    // Allocating the space for the structure
    context = ( struct ctx* )malloc( sizeof( *context ) );
    // Allocating the video buffer
    context->pixels = ( uchar* )malloc( ( sizeof( *( context->pixels ) ) * VIDEO_WIDTH * VIDEO_HEIGHT ) * 4 );
    // Allocating the mutex
    context->mutex = new QMutex();
    context->videoUnderlay = this;

    // Creating some char[] to store the media options
    char clock[64], cunlock[64], cdata[64];
    char width[32], height[32], chroma[32], pitch[32];

    // Preparing the options for the media
    // The clock and cunlock contain a pointer to the associated
    // static method (note the use of %lld).
    //
    // In that specific case we can't use Qt:
    // The sprintf method of the QString does not support
    // length modifiers (like %lld).
    sprintf( clock,   ":vmem-lock=%lld",   (long long int)(intptr_t)lock    );
    sprintf( cunlock, ":vmem-unlock=%lld", (long long int)(intptr_t)unlock  );
    sprintf( cdata,   ":vmem-data=%lld",   (long long int)(intptr_t)context );
    sprintf( width,   ":vmem-width=%i",    VIDEO_WIDTH                      );
    sprintf( height,  ":vmem-height=%i",   VIDEO_HEIGHT                     );
    sprintf( chroma,  ":vmem-chroma=%s",   "RV32"                           );
    sprintf( pitch,   ":vmem-pitch=%i",    VIDEO_WIDTH * 4                  );

    // List of options
    // This part can be easily replaced by a QStringList
    // instead of a C array.
    char const* media_options[] =
    {
        ":vout=vmem",
        width, height,
        chroma, pitch,
        clock, cunlock,
        cdata
    };
    int media_options_size = sizeof( media_options )
                             / sizeof( *media_options );

    // Adding each option from the array to the media
    for ( int i = 0; i < media_options_size; ++i )
    {
        libvlc_media_add_option( m_vlcMedia, media_options[i], &m_ex );
        catchException();
    }

    // Put the media into the mediaplayer
    libvlc_media_player_set_media( m_vlcMediaplayer, m_vlcMedia, &m_ex );
    catchException();

    // generate the content identifier
    generateContentIdentifier();

    play();
    libvlc_state_t currentState;
    while(currentState != libvlc_Playing) {
        currentState = libvlc_media_player_get_state(m_vlcMediaplayer, &m_ex);
    }
    pause();

    // emit the content changed signal
    emit contentChanged(getContentIdentifier());
    emit mediaLengthUpdate(getMediaLength());
    emit timelineUpdate(getMediaPosition());
}

void VideoUnderlay::play()
{
    if ( mrl.isEmpty() ) return;

    // Finally, start the playback.
    libvlc_media_player_play( m_vlcMediaplayer, &m_ex );
    catchException();
}

// TODO create private wrapper functions for things like isPlaying and canPause
void VideoUnderlay::pause()
{
    bool isPlaying = (bool) libvlc_media_player_is_playing(m_vlcMediaplayer, &m_ex);
    catchException();
    bool canPause = (bool) libvlc_media_player_can_pause(m_vlcMediaplayer, &m_ex);
    catchException();
    if(isPlaying && canPause) {
        libvlc_media_player_pause(m_vlcMediaplayer, &m_ex);
        catchException();
    }
}

void VideoUnderlay::stop()
{
    if(libvlc_media_player_is_playing(m_vlcMediaplayer, &m_ex)) {
        libvlc_media_player_stop(m_vlcMediaplayer, &m_ex);
        catchException();
    }
}

void VideoUnderlay::lock( struct ctx* ctx, void** pp_ret )
{
    // Lock the buffer (to avoid concurrent access and data corruption)
    ctx->mutex->lock();
    // Tell libvlc to write the next frame into our pre-allocated buffer
    *pp_ret = ctx->pixels;
}

void VideoUnderlay::unlock( struct ctx* ctx )
{
    // As we are in a static method we don't have an instance
    // of the VideoUnderlay here. Fortunately we created a copy
    // of our instance pointer into the ctx structure,
    // do you remember ?
    ctx->videoUnderlay->emit frameReady( ctx );
    // That way we are able to emit a signal containing our
    // new video frame !
}

void VideoUnderlay::processNewFrame( struct ctx* ctx )
{
    // WARNING:
    // This part of the code is extremely slow and inefficient
    // (but sufficient for our example).

    // The bottleneck starts here ;)

    // Conversion of the frame from our pixel buffer to a QImage
    QImage px( ctx->pixels, VIDEO_WIDTH, VIDEO_HEIGHT, QImage::Format_RGB32 );
    // Then we're creating a new QPixmap from our QImage
    QPixmap pix = QPixmap::fromImage( px );

    // Set the current frame into the video container
    video->setPixmap( pix );

    // Releasing the mutex for the upcoming frame.
    ctx->mutex->unlock();
    // TODO we still keep coming here when we are paused? why?
    handleTimelineChange();
}

bool VideoUnderlay::catchException()
{
    if ( libvlc_exception_raised( &m_ex ) )
    {
        /* For VLC <= 1.0.x */
        errorHandler->showMessage( libvlc_exception_get_message( &m_ex ) );

        /* For VLC >= 1.1.x */
        //errorHandler->showMessage( libvlc_errmsg() );

        libvlc_exception_clear( &m_ex );
        return true;
    }
    return false;
}

bool VideoUnderlay::selectContent(QString location)
{
    loadMedia(location);
    // TODO add to recent list in a more proper way?
    // TODO better way to determine video title?
    QFileInfo f(location);
    recentlyUsed->addRecentItem(f.fileName(), location);
    // TODO error handling - don't always return true
    return true;
}

QString VideoUnderlay::getContentContext()
{
    return QString::number(m_prevTimelineContext) + "_0";
}

void VideoUnderlay::generateContentIdentifier()
{
    contentMD5 = ContentDisplay::generateFileMD5(mrl);
}

void VideoUnderlay::handleTimelineChange()
{
    emit timelineUpdate(getMediaPosition());

    int currentTimelineContext = getMediaPosition() / 1000;

    // if context has changed, update the context info and emit signal
    if(currentTimelineContext != m_prevTimelineContext) {
        m_prevTimelineContext = currentTimelineContext;
        emit contextChanged(getContentContext());
    }
}
