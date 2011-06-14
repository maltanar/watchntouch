#-------------------------------------------------
#
# Project created by QtCreator 2011-04-16T14:28:32
#
#-------------------------------------------------

QT       += core gui network

TARGET = watchntouch_collaboration
TEMPLATE = app

COMMON_SOURCES +=   ../common/wtmessage.cpp \
                     ../common/wtloginmessage.cpp \
                     ../common/wtsessionlistrequest.cpp \
                     ../common/messagetransceiver.cpp \
                     ../common/wtlogoutrequest.cpp \
                     ../common/wtpicturerequest.cpp \
                     ../common/wtpictureresponse.cpp \
		     ../common/wtsessioncreaterequest.cpp \
		     ../common/wtsessioncreateresponse.cpp \
                     ../common/wtsessionjoinrequest.cpp \
                     ../common/wtsessionjoinresponse.cpp \
                     ../common/wtsessionleaverequest.cpp \
                     ../common/wtsessionleaveresponse.cpp \
                     ../common/wtsessionlistresponse.cpp \
                     ../common/wtsessionmemberupdate.cpp \
                     ../common/wtupdatedrawing.cpp \
                     ../common/wtupdatedrawingserver.cpp \
                     ../common/wtwritepermissionrequest.cpp \
                     ../common/wtwritepermissionstatus.cpp \
                     ../common/wtloginresponse.cpp \
                     ../common/protocolhandler.cpp \
                     ../common/wtpeerhandshake.cpp \
                     ../common/collaborationsession.cpp 

CWIDGET_SOURCES += ../collaborative_drawing_widget/basedrawingwidget.cpp \
	               ../collaborative_drawing_widget/drawingdata.cpp \
	               ../collaborative_drawing_widget/collaborativedrawingwidget.cpp \
	               ../collaborative_drawing_widget/drawingaction.cpp                             

COMMON_HEADERS +=    ../common/wtmessage.h \
                     ../common/wtloginmessage.h \
                     ../common/wtsessionlistrequest.h \
                     ../common/messagetransceiver.h \
                     ../common/wtlogoutrequest.h \
                     ../common/wtpicturerequest.h \
                     ../common/wtpictureresponse.h \
		     ../common/wtsessioncreaterequest.h \
		     ../common/wtsessioncreateresponse.h \
                     ../common/wtsessionjoinrequest.h \
                     ../common/wtsessionjoinresponse.h \
                     ../common/wtsessionleaverequest.h \
                     ../common/wtsessionleaveresponse.h \
                     ../common/wtsessionlistresponse.h \
                     ../common/wtsessionmemberupdate.h \
                     ../common/wtupdatedrawing.h \
                     ../common/wtupdatedrawingserver.h \
                     ../common/wtwritepermissionrequest.h \
                     ../common/wtwritepermissionstatus.h \
                     ../common/wtloginresponse.h \
                     ../common/protocolhandler.h \
                     ../common/wtpeerhandshake.h \
                     ../common/collaborationsession.h

CWIDGET_HEADERS += ../collaborative_drawing_widget/basedrawingwidget.h \
	               ../collaborative_drawing_widget/drawingdata.h \
	               ../collaborative_drawing_widget/collaborativedrawingwidget.h \
	               ../collaborative_drawing_widget/drawingaction.h \
	               ../collaborative_drawing_widget/appglobals.h                     

INCLUDEPATH += . \
    ../common \
    ../collaborative_drawing_widget

SOURCES += main.cpp\
        mainwindow.cpp \
	collaborationclient.cpp \
	sessionjoindialog.cpp \
	$$COMMON_SOURCES \
	$$CWIDGET_SOURCES \

HEADERS  += mainwindow.h \
    collaborationclient.h \
    sessionjoindialog.h \
    $$COMMON_HEADERS \
    $$CWIDGET_HEADERS

FORMS    += mainwindow.ui \
    sessionjoindialog.ui

OTHER_FILES += \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/AndroidManifest.xml \
    android/src/eu/licentia/necessitas/industrius/QtSurface.java \
    android/src/eu/licentia/necessitas/industrius/QtApplication.java \
    android/src/eu/licentia/necessitas/industrius/QtActivity.java \
    android/src/eu/licentia/necessitas/ministro/IMinistroCallback.aidl \
    android/src/eu/licentia/necessitas/ministro/IMinistro.aidl \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/AndroidManifest.xml \
    android/src/eu/licentia/necessitas/industrius/QtSurface.java \
    android/src/eu/licentia/necessitas/industrius/QtApplication.java \
    android/src/eu/licentia/necessitas/industrius/QtActivity.java \
    android/src/eu/licentia/necessitas/ministro/IMinistroCallback.aidl \
    android/src/eu/licentia/necessitas/ministro/IMinistro.aidl
