/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#include "wtloginmessage.h"

WTLoginMessage::WTLoginMessage() :
    WTMessage()
{
    command = "LOGINREQ";
}
