#include "presentationdisplaywidget.h"

#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>


RenderedImageCash::RenderedImageCash(QObject *parent) :
    QThread(parent)
{
    currentSlideNo = 0;
    prevSlideNo = 0;
    doc = NULL;
}

void RenderedImageCash::run() {

    qWarning() << "thread basladi";

    doc = Poppler::Document::load(fileName);

    int anumber;
    for(int i = 0; i < 3; i++) {
        anumber = currentSlideNo - 1 + i;
        if(anumber <= 0)
            ;
        else
            cash[i] = doc->page(anumber - 1)->renderToImage(120,120);
    }

    prevSlideNo = currentSlideNo;

    qWarning() << "thred bitti!";

}

