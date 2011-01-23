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
