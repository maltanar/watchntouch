#ifndef CONTENTMATCHER_H
#define CONTENTMATCHER_H

#include <QString>

#include "appglobals.h"

class ContentMatcher
{
public:
    ContentMatcher();
    QString matchingAnnotation(QString contentId, QString context);

private:
    QString annotationDirPath;
    QString annotationPrefix;
    QString annotationExtension;

};

#endif // CONTENTMATCHER_H
