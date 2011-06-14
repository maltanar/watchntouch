/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef PRESENTATIONDISPLAYWIDGET_H
#define PRESENTATIONDISPLAYWIDGET_H

#include "contentdisplay.h"
#include "renderedimagecash.h"
#include "loadrenderedimagestofiles.h"

#include <QLabel>

class PresentationDisplayWidget : public ContentDisplay
{
    Q_OBJECT
public:
    explicit PresentationDisplayWidget(QWidget *parent = 0);
    ~PresentationDisplayWidget();

    // overloaded virtual functions from ContentDisplay
    QString getContentContext();
    bool selectContent(QString location);

protected:

    void keyPressEvent(QKeyEvent *ev);
    void resizeEvent(QResizeEvent *ev);

    void prepareThumbnails();

private:
    int currentSlide;
    int slideCount;
    int index;
    qreal scaleFactor;
    QImage m_currentPageImage;
    RenderedImageCash c;
    LoadRenderedImagesToFiles loader;
    QLabel *m_imageDisplayLabel;

    bool first;
    void generateContentIdentifier();

    Poppler::Document *doc;
    bool loadPDF(QString fileName);
    bool loadScreenShot();
    QString convertToPDF(QString inputFile);

signals:
    void pageNumberChanged(int pageNo, int total);
    void pageThumbnailReady(QString fileName, int pageNo);

public slots:
    void gotoNextSlide();
    void gotoPrevSlide();
    void gotoFirstSlide();
    void gotoLastSlide();
    void gotoSlide(int slideNo);

};

#endif // PRESENTATIONDISPLAYWIDGET_H
