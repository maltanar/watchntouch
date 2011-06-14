/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "colorpicker.h"
#include "ui_colorpicker.h"
#include <QBitmap>
#include <QDebug>
#include <QMouseEvent>

ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorPicker)
{
    ui->setupUi(this);

    m_colorWheelPixmap = ui->colorWheel->pixmap()->scaled(ui->colorWheel->size());

    setMask(m_colorWheelPixmap.createMaskFromColor(Qt::transparent, Qt::MaskInColor));
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

void ColorPicker::mousePressEvent(QMouseEvent *e)
{
    emit colorPicked(m_colorWheelPixmap.toImage().pixel(e->x(), e->y()));
    qWarning() << "color" << m_colorWheelPixmap.toImage().pixel(e->x(), e->y()) << "coords" << e->pos();
}
