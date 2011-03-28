#include "transparentstackedlayout.h"

TransparentStackedLayout :: TransparentStackedLayout (QWidget * parent) : QStackedLayout(parent) {
}

int TransparentStackedLayout :: addWidget (QWidget * w) {
        int r = QStackedLayout :: addWidget (w);
        show_all ();
        return r;
}

int TransparentStackedLayout :: insertWidget (int i, QWidget * w) {
        int r = QStackedLayout :: insertWidget (i, w);
        show_all ();
        return r;
}

void TransparentStackedLayout :: setCurrentIndex (int) {
        // Do nothing. There is no notion of "current" item.
}

void TransparentStackedLayout :: setCurrentItem (QWidget *) {
        // Do nothing. There is no notion of "current" item.
}

void TransparentStackedLayout :: show_all () {
        for (int i=0; i<count(); i++) {
                itemAt(i)->widget()->raise();
                itemAt(i)->widget()->show();
        }
}
