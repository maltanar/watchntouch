#ifndef TRANSPARENTSTACKEDLAYOUT_H
#define TRANSPARENTSTACKEDLAYOUT_H

#include <QStackedLayout>
#include <QWidget>

class TransparentStackedLayout : public QStackedLayout {
        Q_OBJECT
public:
        TransparentStackedLayout (QWidget * =0);

        int addWidget (QWidget *);
        int insertWidget (int, QWidget *);

public slots:
        void setCurrentIndex (int);
        void setCurrentItem (QWidget *);

private:
        //! Shows everything in the widget.
        void show_all ();
};

#endif // TRANSPARENTSTACKEDLAYOUT_H
