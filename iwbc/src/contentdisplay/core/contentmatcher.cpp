#include "contentmatcher.h"

#include <QDir>
#include <QApplication>

#include "appglobals.h"

ContentMatcher::ContentMatcher()
{
    annotationDirPath = ANNOTATION_DIR;
    annotationPrefix = ANNOTATION_PREFIX;
    annotationExtension = ANNOTATION_EXTENSION;

    QDir annotationDir(annotationDirPath);
    // check if annotations directory exists
    if(!annotationDir.exists()) {
        // directory does not exist, create it
        annotationDir.mkdir(annotationDirPath);
    }
}


QString ContentMatcher::matchingAnnotation(QString contentId, QString context)
{
    QString contentDirPath = annotationDirPath + "/" + contentId;
    QDir contentDir(contentDirPath);
    // check if directory for given content ID exists
    if(!contentDir.exists()) {
        // directory does not exist, create it
        contentDir.mkdir(contentDirPath);
    }

    return contentDirPath + "/" + annotationPrefix + context + annotationExtension;
}

void ContentMatcher::clearAllAnnotationsForContent(QString contentId)
{
    QString contentDirPath = annotationDirPath + "/" + contentId;
    QDir contentDir(contentDirPath);
    if(contentDir.exists()) {
        // remove all files inside content directory
        QStringList fileList = contentDir.entryList();
        for(int i = 0; i < fileList.count(); i++)
            QFile::remove(contentDirPath + "/" + fileList.at(i));
    }
}
