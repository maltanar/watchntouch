#include "contentdisplay.h"
#include <QCryptographicHash>
#include <QFile>

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

QString ContentDisplay::getContentContext()
{
    return "0_0";
}

bool ContentDisplay::selectContent(QString location)
{
    contentLocation = location;
    generateContentIdentifier();
    emit contentChanged(getContentIdentifier());
    emit contextChanged(getContentContext());

    return true;
}

void ContentDisplay::generateContentIdentifier()
{
    contentMD5 = generateFileMD5(contentLocation);
}

QString ContentDisplay::generateFileMD5(QString fileName)
{
    QFile contentFile(fileName);
    if(!contentFile.open(QFile::ReadOnly)) {
        // TODO log error
        return QString("");
    }
    QString hashData(QCryptographicHash::hash(contentFile.readAll(), QCryptographicHash::Md5).toHex().constData());
    contentFile.close();

    return hashData;
}
