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

#define CONFIG_DIR              "configuration"

#define ANNOTATION_DIRECTORY    "annotations"
#define ANNOTATION_PREFIX       "annotation_"
#define ANNOTATION_EXTENSION    ".wta"

#define NUM_RECENT_ITEMS        5
#define RECENT_ITEMS_STORAGE    CONFIG_DIR"/recent.txt"

extern class RecentlyUsed *recentlyUsed;
extern class GoogleDocsAccess *googleDocsAccess;


#endif // APPGLOBALS_H
