/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef BOOKMARKLIST_H
#define BOOKMARKLIST_H

#include <QStringList>

class BookmarkList : public QStringList
{
public:
    explicit BookmarkList();
    ~BookmarkList();

    void readFromStorage();
    void writeToStorage();

private:
    QString m_itemStorageFileName;

};

#endif // BOOKMARKLIST_H
