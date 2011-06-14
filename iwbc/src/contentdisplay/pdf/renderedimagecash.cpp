/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "presentationdisplaywidget.h"

#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>
#include <QImageReader>


RenderedImageCash::RenderedImageCash(QObject *parent) :
    QThread(parent)
{
    currentSlideNo = 0;
    slideCount = 0;
    doc = NULL;
    areImagesReady = false;
}

void RenderedImageCash::run() {

    qWarning() << "cash thread basladi";

    doc = Poppler::Document::load(fileName);
    if(!areImagesReady) {
        int anumber;
        for(int i = 0; i < 7; i++) {
            anumber = currentSlideNo - 3 + i;
            qWarning() << "i, anumber: " << i << anumber;
            if(anumber <= 0 || anumber > slideCount)
                ;
            else
                cash[i] = doc->page(anumber - 1)->renderToImage(120,120);
        }
    }
    else {
        qWarning() << "file dan okuyacak";
        int anumber;
        for(int i = 0; i < 7; i++) {
            anumber = currentSlideNo - 3 + i;
            QString s;
            s = s.setNum(anumber);
            QString imageDirPath = IMAGE_DIR;
            imageDirPath.append("/").append(s).append(".png");
            QImageReader r(imageDirPath,"png");
            qWarning() << "i, anumber: " << i << anumber;
            if(anumber <= 0 || anumber > slideCount)
                ;
            else
                r.read(&cash[i]);
        }
    }
    qWarning() << "cash thred bitti!";

}

void RenderedImageCash::imagesAreReady() {
    areImagesReady = true;
}
