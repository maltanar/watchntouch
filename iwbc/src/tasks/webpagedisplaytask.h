#ifndef WEBPAGEDISPLAYTASK_H
#define WEBPAGEDISPLAYTASK_H

#include "contentdisplaytask.h"
#include <QStackedLayout>
#include <webpagedisplaywidget.h>
#include <annotationwidget.h>
#include <QGroupBox>
#include <contextmenu.h>

class WebPageDisplayTask : public ContentDisplayTask
{
    Q_OBJECT
public:
    explicit WebPageDisplayTask(QWidget *parent = 0);
    ContentType getContentType();

    void activate();
    void deactivate();

    int getTaskType();

protected:
    QWidget * m_webCanvas;
    WebpageDisplayWidget * m_webDisplay;
    AnnotationWidget * m_webDraw;

    void showHidePanel(bool show);
    void updateBookmarksList();


    void webGuiAddBookmark(QString URL);
    void webGuiRemoveBookmark(int index);
    void webGuiClearBookmarks();

    void setWebGuiReadOnlyStatus(bool readOnly);
    void setWebGuiURLText(QString urlString);

protected slots:
    void urlChanged(QUrl newUrl);
    void loadProgress(int progress);

signals:

public slots:
    void bookmarkRequest(QString URL);
    void deleteBookmarkRequest(QString URL, int index);


    /*

    signal back()
    signal next()
    signal refresh()
    signal bookmarkRequest(string URL)
    signal gotoBookmark(string URL)
    signal deleteBookmarkSignal(string URL)
    signal bookmarkMenuShowHide(bool showhide)
    signal webAnnotationStatus(bool onOff)
    signal gotoURL(string URL)


    function addBookmark(URL){
        weblistModelSketch.append({"val": URL});
    }

    function deleteBookmark(index){
        weblistModelSketch.remove(index);
    }

    function closeBookmarkList(){
        favListHide.running=true;
    }
*/

};

#endif // WEBPAGEDISPLAYTASK_H
