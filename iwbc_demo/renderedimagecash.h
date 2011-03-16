#ifndef RENDEREDIMAGECASH_H
#define RENDEREDIMAGECASH_H

#include <QThread>
#include <poppler-qt4.h>

#include "contentdisplay.h"

class RenderedImageCash : public QThread
{
    Q_OBJECT

public:
    explicit RenderedImageCash(QObject *parent = 0);
    void run(); // From QThread

    QImage cash[7];
    int currentSlideNo;
    int slideCount;

    Poppler::Document *doc;
    QString fileName;

signals:

public slots:

};

#endif // RENDEREDIMAGECASH_H
