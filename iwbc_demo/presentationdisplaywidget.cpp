#include "presentationdisplaywidget.h"

#include <QDebug>

PresentationDisplayWidget::PresentationDisplayWidget(QWidget *parent) :
    ContentDisplay(parent)
{
    currentSlide = 0;
    slideCount = 0;
    scaleFactor = 1.0;

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

    if(location.endsWith(".pdf")) {
        return loadPDF(location);
    } else if(location.endsWith(".odp")) {
        // TODO handle ODP loading
    } else if(location.endsWith(".ppt")) {
        // TODO handle PPT loading
    } else {
        // TODO display error - unsupported presentation format
        return false;
    }

    return false;
}

bool PresentationDisplayWidget::loadPDF(QString fileName)
{
    // TODO this function will move to PresentationLoader
    Poppler::Document *oldDocument = doc;

    doc = Poppler::Document::load(fileName);
    if (doc) {
        delete oldDocument;
        contentLocation = fileName;
        generateContentIdentifier();
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

    pageImage = doc->page(slideNo-1)->renderToImage(scaleFactor * QLabel::physicalDpiX(), scaleFactor * QLabel::physicalDpiY());

    setPixmap(QPixmap::fromImage(pageImage));

    // TODO do error checking
    currentSlide = slideNo;
    emit contextChanged(getContentContext());
}

void PresentationDisplayWidget::generateContentIdentifier()
{
    contentMD5 = ContentDisplay::generateFileMD5(contentLocation);
}
