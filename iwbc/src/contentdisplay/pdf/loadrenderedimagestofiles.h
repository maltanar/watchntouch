#ifndef LOADRENDEREDIMAGESTOFILES_H
#define LOADRENDEREDIMAGESTOFILES_H

#include <QThread>
#include <poppler-qt4.h>

class LoadRenderedImagesToFiles : public QThread
{
    Q_OBJECT
public:
    explicit LoadRenderedImagesToFiles(QObject *parent = 0);
    void run(); // From QThread

    int slideCount;
    Poppler::Document *doc;
    QString fileName;

signals:
    void imagesAreReady();

public slots:

};

#endif // LOADRENDEREDIMAGESTOFILES_H
