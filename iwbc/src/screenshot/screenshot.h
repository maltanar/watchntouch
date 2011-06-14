/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QDialog>

namespace Ui {
    class Screenshot;
}

class Screenshot : public QDialog
{
    Q_OBJECT

public:
    explicit Screenshot(QWidget *parent = 0);
    ~Screenshot();

public slots:
    void on_scrnshot_clicked();

signals:
    void acquiredScreenshot(QPixmap screenShot);

private:
    Ui::Screenshot *ui;
};

#endif // SCREENSHOT_H
