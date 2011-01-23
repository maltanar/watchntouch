#include "recentlyused.h"

#include <QFile>

RecentlyUsed::RecentlyUsed() :
    QStringList()
{
    capacity = NUM_RECENT_ITEMS;
    itemStorageFileName = qApp->applicationDirPath() + "/" + QString(RECENT_ITEMS_STORAGE);

    readFromStorage();
}

RecentlyUsed::~RecentlyUsed()
{
    writeToStorage();
}

void RecentlyUsed::readFromStorage()
{
    clear();

    QFile storageFile(itemStorageFileName);
    if(!storageFile.exists())
        // no items in recent list
        return;

    storageFile.open(QFile::ReadOnly);
    while(!storageFile.atEnd()) {
        append(QString::fromUtf8(storageFile.readLine()).trimmed());
    }
    storageFile.close();
}

void RecentlyUsed::writeToStorage()
{
    QFile storageFile(itemStorageFileName);
    storageFile.remove();
    storageFile.open(QFile::WriteOnly);
    for(int i=0; i < size(); i++)
        storageFile.write((at(i)+"\n").toUtf8());
    storageFile.close();
}

void RecentlyUsed::addRecentItem(QString title, QString url)
{
    QString newItemText = "%1\t%2";
    newItemText = newItemText.arg(title).arg(url);
    // check to see if list already contains this item
    int oldIndex = lastIndexOf(newItemText);
    if(oldIndex != -1)
        removeAt(oldIndex); // remove old instance
    // place entry at the beginning of the list
    prepend(newItemText);
    // shrink to fit to capacity
    while(size() > capacity)
        removeLast();
}

bool RecentlyUsed::getRecentItem(int index, QString &titleVar, QString &urlVar)
{
    if(index >= size())
        return false;

    QStringList vals =  at(index).split("\t");
    titleVar = vals.at(0);
    urlVar = vals.at(1);

    return true;
}
