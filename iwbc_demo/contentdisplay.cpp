#include "contentdisplay.h"
#include <QCryptographicHash>
#include <QFile>

#include "appglobals.h"

ContentDisplay::ContentDisplay(QWidget *parent) :
    QLabel(parent)
{
    contentType = CONTENTTYPE_UNDEFINED;
    contentSize = QSize(0,0);
}

QString ContentDisplay::getContentIdentifier()
{
    return contentMD5;
}

QSize ContentDisplay::getContentSize()
{
    return contentSize;
}

void ContentDisplay::setContentSize(QSize newSize)
{
    contentSize = newSize;
}

void ContentDisplay::generateContentIdentifier()
{
    contentMD5 = generateFileMD5(contentLocation);
}

QString ContentDisplay::generateFileMD5(QString fileName)
{
    QFile contentFile(fileName);
    if(!contentFile.open(QFile::ReadOnly)) {
        displayErrorMessage("Could not open content file: \n" + fileName);
        return QString("");
    }
    QString hashData(QCryptographicHash::hash(contentFile.readAll(), QCryptographicHash::Md5).toHex().constData());
    contentFile.close();

    return hashData;
}

ContentType ContentDisplay::getContentType()
{
    return contentType;
}

QString ContentDisplay::getContentTitle()
{
    return contentTitle;
}

void ContentDisplay::setDesiredSize(QSize newSize)
{
    desiredSize = newSize;
}

QSize ContentDisplay::getDesiredSize()
{
    return desiredSize;
}
