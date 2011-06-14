/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QColor>

namespace Ui {
    class ColorPicker;
}

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent = 0);
    ~ColorPicker();

protected:
    void mousePressEvent(QMouseEvent *);

private:
    Ui::ColorPicker *ui;
    QPixmap m_colorWheelPixmap;

signals:
    void colorPicked(QColor color);
};

#endif // COLORPICKER_H
