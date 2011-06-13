QT       += core gui xml network webkit declarative

TARGET = iwbc
TEMPLATE = app

LIBS         += -L/usr/lib -lpoppler-qt4 -lwiiuse -lX11 -lXtst -lXext -Wall -pipe -fPIC -funroll-loops -lvlc

COLLABR_SOURCES +=    src/collaboration/wtmessage.cpp \
                      src/collaboration/wtloginmessage.cpp \
                      src/collaboration/wtsessionlistrequest.cpp \
                      src/collaboration/messagetransceiver.cpp \
                      src/collaboration/wtlogoutrequest.cpp \
                      src/collaboration/wtpicturerequest.cpp \
                      src/collaboration/wtpictureresponse.cpp \
		              src/collaboration/wtsessioncreaterequest.cpp \
		              src/collaboration/wtsessioncreateresponse.cpp \
                      src/collaboration/wtsessionjoinrequest.cpp \
                      src/collaboration/wtsessionjoinresponse.cpp \
                      src/collaboration/wtsessionleaverequest.cpp \
                      src/collaboration/wtsessionleaveresponse.cpp \
                      src/collaboration/wtsessionlistresponse.cpp \
                      src/collaboration/wtsessionmemberupdate.cpp \
                      src/collaboration/wtupdatedrawing.cpp \
                      src/collaboration/wtupdatedrawingserver.cpp \
                      src/collaboration/wtwritepermissionrequest.cpp \
                      src/collaboration/wtwritepermissionstatus.cpp \
                      src/collaboration/wtloginresponse.cpp \
                      src/collaboration/protocolhandler.cpp \
                      src/collaboration/wtpeerhandshake.cpp \
                      src/collaboration/collaborationsession.cpp \                 

COLLABR_HEADERS +=    src/collaboration/wtmessage.h \
                      src/collaboration/wtloginmessage.h \
                      src/collaboration/wtsessionlistrequest.h \
                      src/collaboration/messagetransceiver.h \
                      src/collaboration/wtlogoutrequest.h \
                      src/collaboration/wtpicturerequest.h \
                      src/collaboration/wtpictureresponse.h \
		              src/collaboration/wtsessioncreaterequest.h \
		              src/collaboration/wtsessioncreateresponse.h \
                      src/collaboration/wtsessionjoinrequest.h \
                      src/collaboration/wtsessionjoinresponse.h \
                      src/collaboration/wtsessionleaverequest.h \
                      src/collaboration/wtsessionleaveresponse.h \
                      src/collaboration/wtsessionlistresponse.h \
                      src/collaboration/wtsessionmemberupdate.h \
                      src/collaboration/wtupdatedrawing.h \
                      src/collaboration/wtupdatedrawingserver.h \
                      src/collaboration/wtwritepermissionrequest.h \
                      src/collaboration/wtwritepermissionstatus.h \
                      src/collaboration/wtloginresponse.h \
                      src/collaboration/protocolhandler.h \
                      src/collaboration/wtpeerhandshake.h \
                      src/collaboration/collaborationsession.h


INCLUDEPATH  +=     src   \
                    src/contentdisplay    \
                    src/contentdisplay/core   \
                    src/contentdisplay/pdf    \
                    src/contentdisplay/webpage    \
                    src/contentdisplay/video  \
                    src/contentselect \
                    src/drawing   \
                    src/googledocs    \
                    src/screencasting \
                    src/screenshot    \
                    src/wiimote \
                    src/tasks \
                    src/collaboration \
                    /usr/include/poppler/qt4     



FORMS    +=         forms/mainwindow.ui \
                    forms/contentselector.ui \
                    forms/calibrationwindow.ui \
                    forms/contextmenu.ui \
                    forms/screenshot.ui \
                    forms/videocontrolpanel.ui \
                    forms/webcontrolpanel.ui \
                    forms/colorpicker.ui

HEADERS +=          src/qmlmenulayer.h \
                    src/mainwindow.h \
                    src/contextmenu.h \
                    src/appglobals.h \
                    src/contentdisplay/core/contentmatcher.h \
                    src/contentdisplay/core/contentdisplay.h \
                    src/contentdisplay/pdf/renderedimagecash.h \
                    src/contentdisplay/pdf/presentationdisplaywidget.h \
                    src/contentdisplay/pdf/loadrenderedimagestofiles.h \
                    src/contentdisplay/video/videounderlay.h \
                    src/contentdisplay/video/videocontrolpanel.h \
                    src/contentdisplay/webpage/webpagedisplaywidget.h \
                    src/contentdisplay/webpage/webcontrolpanel.h \
                    src/contentselect/recentlyused.h \
                    src/contentselect/contentselector.h \
                    src/drawing/sketchingwidget.h \
                    src/drawing/drawingdata.h \
                    src/drawing/drawingaction.h \
                    src/drawing/basedrawingwidget.h \
                    src/drawing/annotationwidget.h \
                    src/googledocs/googledocsaccess.h \
                    src/googledocs/googledocentry.h \
                    src/screencasting/screencasting.h \
                    src/screenshot/screenshot.h \
                    src/wiimote/QjtMouseGestureFilter.h \
                    src/wiimote/QjtMouseGesture.h \
                    src/wiimote/mousegesturerecognizer.h \
                    src/wiimote/irthread.h \
                    src/wiimote/inputcalibration.h \
                    src/wiimote/eventgenerator.h \
                    src/wiimote/calibrationwindow.h \
                    src/tasks/presentationdisplaytask.h \
                    src/tasks/contentdisplaytask.h \
                    src/tasks/videodisplaytask.h \
                    src/tasks/webpagedisplaytask.h \
                    src/contentdisplay/webpage/bookmarklist.h \
                    src/tasks/sketchingtask.h \
                    src/colorpicker.h \
                    $$COLLABR_HEADERS \
    src/drawing/collaborativedrawingwidget.h \
    src/tasks/collaborativedrawingtask.h \
    src/collaboration/collaborationserver.h \
    src/collaboration/collaborationclient.h

SOURCES +=          src/qmlmenulayer.cpp \
                    src/mainwindow.cpp \
                    src/main.cpp \
                    src/contextmenu.cpp \
                    src/contentdisplay/core/contentmatcher.cpp \
                    src/contentdisplay/core/contentdisplay.cpp \
                    src/contentdisplay/pdf/renderedimagecash.cpp \
                    src/contentdisplay/pdf/presentationdisplaywidget.cpp \
                    src/contentdisplay/pdf/loadrenderedimagestofiles.cpp \
                    src/contentdisplay/video/videounderlay.cpp \
                    src/contentdisplay/video/videocontrolpanel.cpp \
                    src/contentdisplay/webpage/webpagedisplaywidget.cpp \
                    src/contentdisplay/webpage/webcontrolpanel.cpp \
                    src/contentselect/recentlyused.cpp \
                    src/contentselect/contentselector.cpp \
                    src/drawing/sketchingwidget.cpp \
                    src/drawing/drawingdata.cpp \
                    src/drawing/drawingaction.cpp \
                    src/drawing/basedrawingwidget.cpp \
                    src/drawing/annotationwidget.cpp \
                    src/googledocs/googledocsaccess.cpp \
                    src/googledocs/googledocentry.cpp \
                    src/screencasting/screencasting.cpp \
                    src/screenshot/screenshot.cpp \
                    src/wiimote/QjtMouseGestureFilter.cpp \
                    src/wiimote/QjtMouseGesture.cpp \
                    src/wiimote/mousegesturerecognizer.cpp \
                    src/wiimote/irthread.cpp \
                    src/wiimote/inputcalibration.cpp \
                    src/wiimote/eventgenerator.cpp \
                    src/wiimote/calibrationwindow.cpp \
                    src/tasks/presentationdisplaytask.cpp \
                    src/tasks/contentdisplaytask.cpp \
                    src/tasks/videodisplaytask.cpp \
                    src/tasks/webpagedisplaytask.cpp \
                    src/contentdisplay/webpage/bookmarklist.cpp \
                    src/tasks/sketchingtask.cpp \
                    src/colorpicker.cpp \
                    $$COLLABR_SOURCES \
    src/drawing/collaborativedrawingwidget.cpp \
    src/tasks/collaborativedrawingtask.cpp \
    src/collaboration/collaborationserver.cpp \
    src/collaboration/collaborationclient.cpp

RESOURCES +=        resources.qrc

OTHER_FILES +=      qml/wtui.qml                    
