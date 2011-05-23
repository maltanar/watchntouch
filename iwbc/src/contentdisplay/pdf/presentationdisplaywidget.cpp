#include "presentationdisplaywidget.h"
#include "loadrenderedimagestofiles.h"

#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>
#include <cmath>
#include <QImageReader>

#include "recentlyused.h"

PresentationDisplayWidget::PresentationDisplayWidget(QWidget *parent) :
    ContentDisplay(parent)
{    
    currentSlide = 0;
    slideCount = 0;
    scaleFactor = 1.0;
    index = 0;
    contentType = CONTENTTYPE_PRESENTATION;

    doc = NULL;

    first = true;

    QObject::connect(&loader,SIGNAL(imagesAreReady()),&c,SLOT(imagesAreReady()));
}

PresentationDisplayWidget::~PresentationDisplayWidget()
{
    if(doc) delete doc;
}

QString PresentationDisplayWidget::getContentContext()
{
    return QString::number(currentSlide) + "_0";
}

bool PresentationDisplayWidget::selectContent(QString location)
{
    // TODO the identification-conversion functionality should reside in PresentationLoader
    // so the code below is only temporary, it'll move into other classes

    qWarning() << "PresentationDisplayWidget::selectContent requested to open" << location;

    if(location.endsWith(".pdf")) {
        return loadPDF(location);
    } else if(location.endsWith(".odp")) {
        return loadPDF(convertToPDF(location));
    } else if(location.endsWith(".ppt")) {
        return loadPDF(convertToPDF(location));
    } else {
        displayErrorMessage("Unsupported presentation format!");
        return false;
    }

    return false;
}

QString PresentationDisplayWidget::convertToPDF(QString inputFile)
{
    // try to convert the given file to PDF
    QFileInfo f(inputFile);
    QString inputFileChecksum = ContentDisplay::generateFileMD5(inputFile);
    QString targetFile = CACHE_DIR + "/" + f.baseName() + "_" + inputFileChecksum + ".pdf";


    // TODO check if OpenOffice exists first!
    // TODO should we do this restart every time?
    // start the OpenOffice.org server
    QProcess openOfficeService;
    openOfficeService.start("soffice -invisible -accept=\"socket,port=8100;urp;\"");

    qWarning() << "target filename for conversion: " << targetFile;


    if(!QFile(targetFile).exists()) {
        // target PDF file does not exist, we didn't convert this before or removed
        // it from the cache, so now we have to convert again
        QStringList args;
        args.append(DOC_CONVERTER_PATH);
        args.append(inputFile);
        args.append(targetFile);

        QProcess p;
        // TODO display error message if python or DocumentConverter.py does not exist
        p.start("python", args);
        if(p.state() == QProcess::NotRunning) {
            displayErrorMessage("Error while trying to convert document to PDF");
            return "";
        }
        // TODO display some sort of progress message
        p.waitForFinished(-1);

        QString output = p.readAllStandardOutput() + " " + p.readAllStandardError();

        if(output.trimmed() != "") {
            // some error occured during the conversion
            displayErrorMessage("An error occured while trying to convert this file:\n"+output.trimmed());
            return "";
        } else {
            // the conversion was successful
            return targetFile;
        }
    } else
        return targetFile; // converted file already exists
}

bool PresentationDisplayWidget::loadPDF(QString fileName)
{
    // TODO this function will move to PresentationLoader
    Poppler::Document *oldDocument = doc;

    if(fileName == "")
        return false;

    doc = Poppler::Document::load(fileName);    
    if (doc) {
        loader.fileName = c.fileName = fileName;
        first = true;
        c.areImagesReady = false;

        delete oldDocument;
        contentLocation = fileName;
        generateContentIdentifier();
        contentSize = doc->page(0)->pageSize();
        contentTitle = doc->info("Title");
        if(contentTitle == "")
            contentTitle = fileName.right(fileName.length()-fileName.lastIndexOf("/"));
        // TODO add to the recent list in a more proper place
        recentlyUsed->addRecentItem(contentTitle, fileName);
        emit contentChanged(getContentIdentifier());
        doc->setRenderHint(Poppler::Document::Antialiasing);
        doc->setRenderHint(Poppler::Document::TextAntialiasing);
        currentSlide = 0;
        loader.slideCount = c.slideCount = slideCount = doc->numPages();
        loader.start();
        gotoSlide(1);
    }
    return doc != 0;
}

void PresentationDisplayWidget::gotoNextSlide()
{
    gotoSlide(currentSlide+1 > slideCount ?  slideCount : currentSlide + 1);
}

void PresentationDisplayWidget::gotoPrevSlide()
{
    gotoSlide(currentSlide-1 < 1 ?  1: currentSlide - 1);
}

void PresentationDisplayWidget::gotoSlide(int slideNo)
{
    if(slideNo == currentSlide)
        return;

    qWarning() << "---------- SLIDE NO " << slideNo << " ---------------";

    // TODO retrieve page from PresentationLoader instead
    // TODO fit to width / height options while rendering need consideration,

    bool isInCash = false;
    for(int i = -3 ; i < 4; i++) {
        if(index + i == slideNo)
            isInCash = true;
    }

    QImage pageImage;

    if(first) {
        qWarning() << "ilk girdi";
        first = false;
        pageImage = doc->page(slideNo-1)->renderToImage(scaleFactor * QLabel::physicalDpiX(),
                                                               scaleFactor * QLabel::physicalDpiY());

        qWarning() << "at first, pageimage size" << pageImage.size() << "desired size" << desiredSize;
        if(desiredSize != QSize(0,0))
            pageImage = pageImage.scaled(desiredSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        contentSize = pageImage.size();

        setPixmap(QPixmap::fromImage(pageImage));

        // TODO do error checking
        currentSlide = slideNo;
        emit contextChanged(getContentContext());

        // change cash accordingly
        c.currentSlideNo = index = slideNo;
        c.start();
        qWarning() << "ilk cikti";

    }
    else {
        if(!c.isRunning()) {
            qWarning() << "c run etmiyor girdi";

            if(isInCash) {
                qWarning() << "cash ten aldi " << slideNo;
                pageImage = c.cash[3 + slideNo - index];
            }
            else {
                qWarning() << "tekrar yuklenecek";
                pageImage = doc->page(slideNo-1)->renderToImage(scaleFactor * QLabel::physicalDpiX(),
                                                                       scaleFactor * QLabel::physicalDpiY());
                // change cash accordingly
                c.currentSlideNo = index = slideNo;
                c.start();
            }

            if(desiredSize != QSize(0,0))
                pageImage = pageImage.scaled(desiredSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            contentSize = pageImage.size();

            setPixmap(QPixmap::fromImage(pageImage));

            // TODO do error checking
            currentSlide = slideNo;
            emit contextChanged(getContentContext());

            qWarning() << "c run etmiyor cikti";

        }
        else {
            qWarning() << "c run ediyor girdi";

            pageImage = doc->page(slideNo-1)->renderToImage(120,120);

            if(desiredSize != QSize(0,0))
                pageImage = pageImage.scaled(desiredSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            contentSize = pageImage.size();

            setPixmap(QPixmap::fromImage(pageImage));

            // TODO do error checking
            currentSlide = slideNo;
            emit contextChanged(getContentContext());

            qWarning() << "c run ediyor cikti";
        }
    }

}

void PresentationDisplayWidget::generateContentIdentifier()
{
    contentMD5 = ContentDisplay::generateFileMD5(contentLocation);
}
