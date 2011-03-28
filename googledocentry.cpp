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
