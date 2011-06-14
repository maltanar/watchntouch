#include "wtlogoutrequest.h"

WTLogoutRequest::WTLogoutRequest() :
    WTMessage()
{
    command = "LOGOUTRQ";
}
