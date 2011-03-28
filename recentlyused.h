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



signals:

public slots:

};

#endif // RECENTLYUSED_H
