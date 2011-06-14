#-------------------------------------------------
#
# Project created by QtCreator 2011-04-16T14:28:32
#
#-------------------------------------------------

QT       += core gui network

TARGET = collaboration_server
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
                     ../common/wtwritepermissionrequest.cpp \
                     ../common/wtwritepermissionstatus.cpp \
                     ../common/wtloginresponse.cpp \
                     ../common/protocolhandler.cpp \
                     ../common/wtpeerhandshake.cpp \
                     ../common/collaborationsession.cpp \
                     ../common/wtupdatedrawingserver.cpp \

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
                     ../common/wtwritepermissionrequest.h \
                     ../common/wtwritepermissionstatus.h \
                     ../common/wtloginresponse.h \
                     ../common/protocolhandler.h \
                     ../common/wtpeerhandshake.h \
                     ../common/collaborationsession.h \
                     ../common/wtupdatedrawingserver.h

SOURCES += main.cpp \
           collaborationserver.cpp \
           $$COMMON_SOURCES


HEADERS  += collaborationserver.h \
            $$COMMON_HEADERS

INCLUDEPATH += . \
    ../common
