#include "presentationdisplaywidget.h"

#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>


RenderedImageCash::RenderedImageCash(QObject *parent) :
    QThread(parent)
{
    currentSlideNo = 0;
    slideCount = 0;
    doc = NULL;
}

void RenderedImageCash::run() {

    qWarning() << "thread basladi";

    doc = Poppler::Document::load(fileName);

    int anumber;
    for(int i = 0; i < 7; i++) {
        anumber = currentSlideNo - 3 + i;
        qWarning() << "i, anumber: " << i << anumber;
        if(anumber <= 0 || anumber > slideCount)
            ;
        else
            cash[i] = doc->page(anumber - 1)->renderToImage(120,120);
    }

    qWarning() << "thred bitti!";

}

