#include "presentationdisplaywidget.h"

#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>

PresentationDisplayWidget::PresentationDisplayWidget(QWidget *parent) :
    ContentDisplay(parent)
{
    currentSlide = 0;
    slideCount = 0;
    scaleFactor = 1.0;
    contentType = CONTENTTYPE_PRESENTATION;

    doc = NULL;
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
        // TODO display error - unsupported presentation format
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
        args.append(TOOLS_DIR + "/DocumentConverter.py");
        args.append(inputFile);
        args.append(targetFile);

        QProcess p;
        // TODO display error message if python or DocumentConverter.py does not exist
        p.start("python", args);
        // TODO display some sort of progress message
        p.waitForFinished(-1);

        QString output = p.readAllStandardOutput() + " " + p.readAllStandardError();

        if(output.trimmed() != "") {
            // some error occured during the conversion
            // display error message
            QMessageBox errMsg;
            errMsg.setText("An error occured while trying to open this file:\n"+output.trimmed());
            errMsg.show();
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
        slideCount = doc->numPages();
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

    QImage pageImage;

    // TODO retrieve page from PresentationLoader instead
    // TODO fit to width / height options while rendering need consideration

    pageImage = doc->page(slideNo-1)->renderToImage(scaleFactor * QLabel::physicalDpiX(),
                                                    scaleFactor * QLabel::physicalDpiY());

    pageImage = pageImage.scaled(desiredSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    contentSize = pageImage.size();

    setPixmap(QPixmap::fromImage(pageImage));

    // TODO do error checking
    currentSlide = slideNo;
    emit contextChanged(getContentContext());
}

void PresentationDisplayWidget::generateContentIdentifier()
{
    contentMD5 = ContentDisplay::generateFileMD5(contentLocation);
}
