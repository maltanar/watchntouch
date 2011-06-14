/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "appglobals.h"
#include "bookmarklist.h"

#include <QFile>

BookmarkList::BookmarkList() :
    QStringList()
{
    m_itemStorageFileName = QString(BOOKMARKS_STORAGE);

    readFromStorage();
}

BookmarkList::~BookmarkList()
{
    writeToStorage();
}

void BookmarkList::writeToStorage()
{
    QFile storageFile(m_itemStorageFileName);
    storageFile.remove();
    storageFile.open(QFile::WriteOnly);
    for(int i=0; i < size(); i++)
        storageFile.write((at(i)+"\n").toUtf8());
    storageFile.close();
}

void BookmarkList::readFromStorage()
{
    clear();

    QFile storageFile(m_itemStorageFileName);
    if(!storageFile.exists())
        // no items in recent list
        return;

    storageFile.open(QFile::ReadOnly);
    while(!storageFile.atEnd()) {
        append(QString::fromUtf8(storageFile.readLine()).trimmed());
    }
    storageFile.close();
}
