#ifndef CONTENTMATCHER_H
#define CONTENTMATCHER_H

#include <QString>

class ContentMatcher
{
public:
    ContentMatcher();
    QString matchingAnnotation(QString contentId, QString context);
    void clearAllAnnotationsForContent(QString contentId);

private:
    QString annotationDirPath;
    QString annotationPrefix;
    QString annotationExtension;

};

#endif // CONTENTMATCHER_H
