/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "screenshot.h"
#include "ui_screenshot.h"

#include <QDesktopWidget>

Screenshot::Screenshot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Screenshot)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    QSize ds = qApp->desktop()->size();
    move(ds.width()-width(), ds.height()-height());
}

Screenshot::~Screenshot()
{
    delete ui;
}

void Screenshot::on_scrnshot_clicked()
{
    hide();
    sleep(1);

    usleep(5000);

    QPixmap thePixmap = QPixmap::grabWindow(QApplication::desktop()->winId());

    emit acquiredScreenshot(thePixmap);
}
