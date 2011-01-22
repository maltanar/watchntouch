#ifndef CONTENTDISPLAY_H
#define CONTENTDISPLAY_H

#include <QLabel>

#include "appglobals.h"

class ContentDisplay : public QLabel
{
    Q_OBJECT
public:
    explicit ContentDisplay(QWidget *parent = 0);

    QString getContentIdentifier(); // return MD5 hash of content
    QSize getContentSize(); // return dimensions of content
    ContentType getContentType(); // return type of content

    // the virtual functions below will be overloaded by ContentDisplay subclasses
    virtual QString getContentContext() = 0; // content context system - see SDD 4.1.2
    virtual bool selectContent(QString location) = 0;   // content selection and loading

protected:
    QString contentMD5;
    QString contentLocation;
    QSize   contentSize;
    ContentType contentType;

    virtual void generateContentIdentifier();
    QString generateFileMD5(QString fileName);

signals:
    void contextChanged(QString newContext);    // emitted when context is changed
    void contentChanged(QString newContentId);  // emitted when content file changes

public slots:

};

#endif // CONTENTDISPLAY_H
