#ifndef APPGLOBALS_H
#define APPGLOBALS_H

#include <QDebug>

typedef enum _ContentType {
    CONTENTTYPE_UNDEFINED,
    CONTENTTYPE_PRESENTATION,
    CONTENTTYPE_VIDEO,
    CONTENTTYPE_WEBPAGE
} ContentType;

#define ANNOTATION_DIRECTORY    "annotations"
#define ANNOTATION_PREFIX       "annotation_"
#define ANNOTATION_EXTENSION    ".wta"


#endif // APPGLOBALS_H
