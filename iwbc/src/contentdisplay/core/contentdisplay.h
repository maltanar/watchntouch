#ifndef CONTENTDISPLAY_H
#define CONTENTDISPLAY_H
#include "appglobals.h"
#include <QLabel>


class ContentDisplay : public QLabel
{
    Q_OBJECT
public:
    explicit ContentDisplay(QWidget *parent = 0);

    QString getContentIdentifier(); // return MD5 hash of content
    QSize getContentSize(); // return dimensions of content
    void setContentSize(QSize newSize);
    ContentType getContentType(); // return type of content
    QString getContentTitle();  // return title of content

    // the virtual functions below will be overloaded by ContentDisplay subclasses
    virtual QString getContentContext() = 0; // content context system - see SDD 4.1.2
    virtual bool selectContent(QString location) = 0;   // content selection and loading

    void setDesiredSize(QSize newSize);
    QSize getDesiredSize();

protected:
    QString contentMD5;
    QString contentLocation;
    QSize   contentSize;
    QString contentTitle;
    QSize desiredSize;
    ContentType contentType;

    virtual void generateContentIdentifier();
    QString generateFileMD5(QString fileName);

signals:
    void contextChanged(QString newContext);    // emitted when context is changed
    void contentChanged(QString newContentId);  // emitted when content file changes
    void requestReadOnlyAnnotation(bool readOnly);
    void scrollRequested(int dx, int dy);

public slots:

};

#endif // CONTENTDISPLAY_H
