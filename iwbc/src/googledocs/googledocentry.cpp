/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "googledocentry.h"

GoogleDocEntry::GoogleDocEntry()
{
    documentTitle = "";
    resourceId = "";
}

void GoogleDocEntry::importFromXMLNode(QDomNode element)
{
    documentTitle = element.firstChildElement("title").text();
    resourceId = element.firstChildElement("gd:resourceId").text();
}
