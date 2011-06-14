/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef GOOGLEDOCENTRY_H
#define GOOGLEDOCENTRY_H

#include <QString>
#include <QDomDocument>

// simple container class for data pertaining to a single Google Document

class GoogleDocEntry
{
public:
    GoogleDocEntry();

    QString documentTitle;
    QString resourceId;

    // TODO add other needed fields

    void importFromXMLNode(QDomNode element);

};

#endif // GOOGLEDOCENTRY_H
