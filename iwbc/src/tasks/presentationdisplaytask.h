#ifndef PRESENTATIONDISPLAYTASK_H
#define PRESENTATIONDISPLAYTASK_H

#include <QStackedLayout>
#include <QStringList>
#include <presentationdisplaywidget.h>
#include <annotationwidget.h>
#include <contextmenu.h>
#include "contentdisplaytask.h"


class PresentationDisplayTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit PresentationDisplayTask(QWidget *parent = 0);

    ContentType getContentType();

    void activate();
    void deactivate();

    int getTaskType();

protected:
    QStackedLayout m_layout;
    QStringList m_thumbs;
    int m_currentPage, m_pageCount;

    void showHidePanel(bool show);
    int getPanelHeight();

    void updateThumbnails();

    // QML status control functions
    void setSlideNumberDisplay(QString text);

    void PresentationGui_clearPageScroller();
    void PresentationGui_alignPageScrollerToPageNumber(int pageNo);
    void PresentationGui_setPageScrollerImageAtIndex(QString pathOfTheImage, int pageNo);
    void PresentationGui_addToPageScroller(QString path, int pageNo);

signals:

protected slots:
    void swipeGesture(int direction, int dx, int dy);

public slots:

    // Incoming information from QML
    void goToPageNumber(QString no);

    // Incoming information from PresentationDisplayWidget
    void pageNumberChanged(int pageNo, int pageCount);
    void pageThumbReady(QString path, int pageNo);

    // TODO IMPORTANT finalize QML connections for presentations
/*    signal goToFirstPage()
    signal goToPrevPage()
    signal goToNextPage()
    signal goToLastPage()

    signal presZoomIn()
    signal presZoomOut()
    signal fitPageWidthToScreenSize();
    signal fitPageHeighttoScreenSize();


    function setSlideNumber(no){
        presPageNumber.text=no;
    }

    function addToPageScroller(pathOfTheImage, pageNo){
        presPagingVisualsListModel.append({"file": pathOfTheImage, "pageNo": pageNo})
    }

    function clearPageScroller(){
        presPagingVisualsListModel.clear();
    }

    function alignPageScrollerToPageNumber(pageNo){
        presPagingVisualsView.positionViewAtIndex(pageNo - 1,ListView.Beginning);
    }
*/
};

#endif // PRESENTATIONDISPLAYTASK_H
