#include <QDebug>
#include <QPainter>
#include <qevent.h>
#include <cmath>

#include "irthread.h"
#include "calibrationwindow.h"
// TODO this class should be renamed to InputReceiver


IRThread::IRThread()
{
    isConnected = 0;
    found = 0;
}


IRThread::~IRThread()
{
}

void IRThread::initialize()
{
    // wiiuse setup
    /*
    *	Initialize an array of wiimote objects.
    *
    *	The parameter is the number of wiimotes I want to create.
    */

    wiimotes =  wiiuse_init(MAX_WIIMOTES);

    /*
    *	Find wiimote devices
    *
    *	Now we need to find some wiimotes.
    *	Give the function the wiimote array we created, and tell it there
    *	are MAX_WIIMOTES wiimotes we are interested in.
    *
    *	Set the timeout to be 5 seconds.
    *
    *	This will return the number of actual wiimotes that are in discovery mode.
    */

    found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
    if (!found) {
            printf ("No wiimotes found.");
            exit(-1);
    }
    /*
    *	Connect to the wiimotes
    *
    *	Now that we found some wiimotes, connect to them.
    *	Give the function the wiimote array and the number
    *	of wiimote devices we found.
    *
    *	This will return the number of established connections to the found wiimotes.
    */

    isConnected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
    if (isConnected) {
            printf("isConnected to %i wiimotes (of %i found).\n", isConnected, found);
            emit connected();
    }
    else {
            printf("Failed to connect to any wiimote.\n");
            return;
    }

    /*
    *	Now set the LEDs and rumble for a second so it's easy
    *	to tell which wiimotes are connected (just like the wii does).
    */


    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
    wiiuse_rumble(wiimotes[0], 1);

    #ifndef WIN32
            usleep(200000);
    #else
            Sleep(200);
    #endif

    wiiuse_rumble(wiimotes[0], 0);
    wiiuse_set_ir(wiimotes[0], 1);
    // enable motion sensing to get the IR point nonvisible transition events
    wiiuse_motion_sensing(wiimotes[0], 1);

    //wiiuse_set_ir_sensitivity(wiimotes[0], 2);

    previousPoint.setX(0);
    previousPoint.setY(0);

    previous[0] = false;
}

// TODO aslında şu anda tek tür event gönderimi yapıyoruz, sadece IR point görünüyorken.
// onun yerine şu yaklaşımı izlesek faydalı olacak aslında:
// - daha önce görünmeyen bir yerde (prevX ve prevY ye bakarak anlaşılır) IR point çıktıysa TouchBegin
// - daha önce görünen bi yerin yakınlarında tekrar IR point oluştuysa TouchMove
// - daha önce görünen bi yerde artık IR point görünmüyorsa TouchEnd
// bunu EventGenerator içinde halletmek mantıklı.

void IRThread::run()
{
    int visiblePointCount = 0;
    while(!isConnected)
        initialize();

    int poll_res = 0;
    while(1) {
        poll_res = wiiuse_poll(wiimotes, MAX_WIIMOTES);
        //qWarning() << "poll result" << poll_res;
        if (poll_res) {
            //qWarning() << "wii event: " << wiimotes[0]->event;
            if(wiimotes[0]->event == WIIUSE_EVENT) {
                //qWarning() << "1";
                if (WIIUSE_USING_IR(wiimotes[0])) {
                    int i = 0;
                    //count number of visible points
                    visiblePointCount = 0;
                    for(int i = 0; i < 4; i++)
                        if(wiimotes[0]->ir.dot[i].visible)
                            visiblePointCount++;
                    // process ir dot #0
                    if (wiimotes[0]->ir.dot[0].visible) {
                        if(previous[0] == false) {
                            previous[0] = true;
                            emit IRInputReceived(wiimotes[0]->ir.dot[0].rx, wiimotes[0]->ir.dot[0].ry,0,MOUSE_PRESSED,visiblePointCount);
                            previousPoint = QPoint(wiimotes[0]->ir.dot[0].rx,wiimotes[0]->ir.dot[0].ry);
                        }
                        else if(previous[0] == true) {
                            emit IRInputReceived(wiimotes[0]->ir.dot[0].rx, wiimotes[0]->ir.dot[0].ry,0,MOUSE_MOVE,visiblePointCount);
                            previousPoint = QPoint(wiimotes[0]->ir.dot[0].rx,wiimotes[0]->ir.dot[0].ry);
                        }
                    }
                    else if(previous[0] == true) {
                        previous[0] = false;
                        //emit IRInputReceived(wiimotes[0]->ir.dot[0].rx, wiimotes[0]->ir.dot[0].ry,0,MOUSE_RELEASED,visiblePointCount);
                        //previousPoint = QPoint(wiimotes[0]->ir.dot[0].rx,wiimotes[0]->ir.dot[0].ry);
                        emit IRInputReceived(previousPoint.x(), previousPoint.y(), 0, MOUSE_RELEASED, visiblePointCount);
                    }
                    else if(previous[0] == false) {
                        // do nothing
                    }
                }
            }
        }
    }

    exec();
}

















