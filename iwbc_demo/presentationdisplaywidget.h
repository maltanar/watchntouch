#ifndef PRESENTATIONDISPLAYWIDGET_H
#define PRESENTATIONDISPLAYWIDGET_H

#include "contentdisplay.h"
#include "renderedimagecash.h"
#include <poppler-qt4.h>

class PresentationDisplayWidget : public ContentDisplay
{
    Q_OBJECT
public:
    explicit PresentationDisplayWidget(QWidget *parent = 0);
    ~PresentationDisplayWidget();

    // overloaded virtual functions from ContentDisplay
    QString getContentContext();
    bool selectContent(QString location);

private:
    int currentSlide;
    int slideCount;
    qreal scaleFactor;
    RenderedImageCash c;
    bool first;
    void generateContentIdentifier();

    Poppler::Document *doc;
    bool loadPDF(QString fileName);
    QString convertToPDF(QString inputFile);

signals:

public slots:
    void gotoNextSlide();
    void gotoPrevSlide();
    void gotoSlide(int slideNo);

};

#endif // PRESENTATIONDISPLAYWIDGET_H
