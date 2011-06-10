#ifndef WEBPAGEDISPLAYTASK_H
#define WEBPAGEDISPLAYTASK_H

#include "contentdisplaytask.h"
#include <QStackedLayout>
#include <webpagedisplaywidget.h>
#include <webcontrolpanel.h>
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

protected:
    QWidget * m_webCanvas;
    WebpageDisplayWidget * m_webDisplay;
    AnnotationWidget * m_webDraw;
    WebControlPanel * m_webControlPanel;

    void showHidePanel(bool show);

    void setWebGuiReadOnlyStatus(bool readOnly);
    void setWebGuiURLText(QString urlString);

protected slots:
    void urlChanged(QUrl newUrl);

signals:

public slots:


    /*

    signal back()
    signal next()
    signal refresh()
    signal bookmarkRequest(string URL)
    signal gotoBookmark(string URL)
    signal deleteBookmarkSignal(string id)
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
