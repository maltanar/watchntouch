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
