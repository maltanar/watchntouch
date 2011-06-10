#ifndef PRESENTATIONDISPLAYTASK_H
#define PRESENTATIONDISPLAYTASK_H

#include "contentdisplaytask.h"
#include <QStackedLayout>
#include <presentationdisplaywidget.h>
#include <annotationwidget.h>
#include <contextmenu.h>

class PresentationDisplayTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit PresentationDisplayTask(QWidget *parent = 0);

    ContentType getContentType();

    void activate();
    void deactivate();

protected:
    QStackedLayout m_layout;

    void showHidePanel(bool show);

    // QML status control functions
    void setSlideNumberDisplay(QString text);


signals:

public slots:

    // Incoming information from QML
    void goToPageNumber(QString no);

    // Incoming information from PresentationDisplayWidget
    void pageNumberChanged(int pageNo, int pageCount);

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
