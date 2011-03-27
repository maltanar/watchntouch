#ifndef APPGLOBALS_H
#define APPGLOBALS_H

#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

typedef enum _ContentType {
    CONTENTTYPE_UNDEFINED,
    CONTENTTYPE_PRESENTATION,
    CONTENTTYPE_VIDEO,
    CONTENTTYPE_WEBPAGE
} ContentType;

// W&T specific directories
#define CONFIG_DIR              qApp->applicationDirPath() + "/" + QString("configuration")
#define CACHE_DIR               qApp->applicationDirPath() + "/" + QString("cache")
#define TOOLS_DIR               qApp->applicationDirPath() + "/" + QString("tools")

#define ANNOTATION_DIR          qApp->applicationDirPath() + "/" + QString("annotations")
#define IMAGE_DIR               qApp->applicationDirPath() + "/" + QString("images");   // for rendered images.
#define SKETCH_DIR              qApp->applicationDirPath() + "/" + QString("sketch");   // for sketch.
#define SCREENSHOT_DIR          qApp->applicationDirPath() + "/" + QString("screenshot");   // for sketch.

// constants and/or shortcuts
#define SCREEN_WIDTH            qApp->desktop()->width()
#define SCREEN_HEIGHT           qApp->desktop()->height()

#define ERASER_SIZE             5

#define MOUSE_PRESSED   0
#define MOUSE_MOVE      1
#define MOUSE_RELEASED  2

#define ANNOTATION_PREFIX       QString("annotation_")
#define ANNOTATION_EXTENSION    QString(".wta")

#define DOC_CONVERTER_FNAME     QString("DocumentConverter.py")
#define DOC_CONVERTER_PATH      TOOLS_DIR + QString("/") + DOC_CONVERTER_FNAME
#define DOC_CONVERTER_RES       QString(":/tools/") + DOC_CONVERTER_FNAME

#define NUM_RECENT_ITEMS        5
#define RECENT_ITEMS_STORAGE    CONFIG_DIR + QString("/recent.txt")

extern class RecentlyUsed *recentlyUsed;
extern class GoogleDocsAccess *googleDocsAccess;
extern class EventGenerator *eventGenerator;

// define the default tools for drawing
#define DEFAULT_DRAWING_PEN     QPen(QBrush(Qt::black, Qt::SolidPattern), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
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
