#ifndef RENDEREDIMAGECASH_H
#define RENDEREDIMAGECASH_H

#include <QThread>
#include <QImage>
#include <poppler-qt4.h>


class RenderedImageCash : public QThread
{
    Q_OBJECT

public:
    explicit RenderedImageCash(QObject *parent = 0);
    void run(); // From QThread

    QImage cash[7];
    int currentSlideNo;
    int slideCount;
    bool areImagesReady;

    Poppler::Document *doc;
    QString fileName;

signals:

public slots:
    void imagesAreReady();

};

#endif // RENDEREDIMAGECASH_H
