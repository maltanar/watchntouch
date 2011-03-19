#include <QImageWriter>
#include <QDir>
#include "loadrenderedimagestofiles.h"

LoadRenderedImagesToFiles::LoadRenderedImagesToFiles(QObject *parent) :
    QThread(parent)
{     
    slideCount = 0;
    doc = NULL;
}


void LoadRenderedImagesToFiles::run() {

    qWarning() << "loader thread basladi";

    doc = Poppler::Document::load(fileName);

    QString imageDirPath = IMAGE_DIR;
    QDir imageDir(imageDirPath);
    if(!imageDir.exists()) {
        imageDir.mkdir(imageDirPath);
    }
    else {
        QStringList l = imageDir.entryList();
        int size = l.size();
        for(int i = 0; i < size; i++) {
            imageDir.remove(l[i]);
        }
    }

    for(int i = 0; i < slideCount; i++) {
        QImage image = doc->page(i)->renderToImage(120,120);
        QString s;
        s = s.setNum(i+1);
        QString path;
        path.append(imageDirPath).append("/").append(s).append(".png");
        QImageWriter writer(path,"png");
        writer.write(image);
    }

    emit imagesAreReady();

    qWarning() << "loader thred bitti!";

}
