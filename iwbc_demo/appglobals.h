#ifndef APPGLOBALS_H
#define APPGLOBALS_H

#include <QDebug>

typedef enum _ContentType {
    CONTENTTYPE_UNDEFINED,
    CONTENTTYPE_PRESENTATION,
    CONTENTTYPE_VIDEO,
    CONTENTTYPE_WEBPAGE
} ContentType;

// TODO make sure config dir exists
#define CONFIG_DIR              "configuration"

#define ANNOTATION_DIRECTORY    "annotations"
#define ANNOTATION_PREFIX       "annotation_"
#define ANNOTATION_EXTENSION    ".wta"

#define NUM_RECENT_ITEMS        5
#define RECENT_ITEMS_STORAGE    CONFIG_DIR"/recent.txt"




#endif // APPGLOBALS_H
