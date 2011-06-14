/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef RECENTLYUSED_H
#define RECENTLYUSED_H

#include <QStringList>
#include <QApplication>


class RecentlyUsed : public QStringList
{
public:
    explicit RecentlyUsed();
    ~RecentlyUsed();
    void readFromStorage();
    void writeToStorage();
    void addRecentItem(QString title, QString url);
    bool getRecentItem(int index, QString &titleVar, QString &urlVar);

private:
    int capacity;
    QString itemStorageFileName;

};

#endif // RECENTLYUSED_H
