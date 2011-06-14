/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

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
