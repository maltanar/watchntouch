#ifndef APPGLOBALS_H
#define APPGLOBALS_H

#include <QDebug>
#include <recentlyused.h>
#include <googledocsaccess.h>

typedef enum _ContentType {
    CONTENTTYPE_UNDEFINED,
    CONTENTTYPE_PRESENTATION,
    CONTENTTYPE_VIDEO,
    CONTENTTYPE_WEBPAGE
} ContentType;

#define CONFIG_DIR              qApp->applicationDirPath() + "/" + QString("configuration")
#define CACHE_DIR               qApp->applicationDirPath() + "/" + QString("cache")
#define TOOLS_DIR               qApp->applicationDirPath() + "/" + QString("tools")

#define ANNOTATION_DIR          qApp->applicationDirPath() + "/" + QString("annotations")
#define ANNOTATION_PREFIX       QString("annotation_")
#define ANNOTATION_EXTENSION    QString(".wta")

#define NUM_RECENT_ITEMS        5
#define RECENT_ITEMS_STORAGE    CONFIG_DIR + QString("/recent.txt")

extern class RecentlyUsed *recentlyUsed;
extern class GoogleDocsAccess *googleDocsAccess;

// define the default tools for drawing
#define DEFAULT_DRAWING_PEN     QPen(Qt::black)
#define DEFAULT_DRAWING_BRUSH   QBrush(Qt::transparent, Qt::SolidPattern)
#define DEFAULT_ERASER          QPen(Qt::red)

typedef enum _DrawingMode {
    DRAWINGMODE_FREEHAND,
    DRAWINGMODE_ERASER,
    DRAWINGMODE_STRAIGHTLINE,
    DRAWINGMODE_ARROW,
    DRAWINGMODE_RECTANGLE,
    DRAWINGMODE_ELLIPSE
} DrawingMode;

typedef enum _DrawingState {
    DRAWINGSTATE_START,
    DRAWINGSTATE_UPDATE,
    DRAWINGSTATE_END
} DrawingState;


#endif // APPGLOBALS_H
