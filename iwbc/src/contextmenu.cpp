/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#include "contextmenu.h"
#include "ui_contextmenu.h"

#include "appglobals.h"

ContextMenu::ContextMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContextMenu)
{
    ui->setupUi(this);

    ui->shapeBox->hide();
    ui->colorBox->hide();
    ui->wtBox->hide();
    ui->penWidthBox->hide();

    m_colorPicker = new ColorPicker(parent);
    m_colorPicker->hide();

    connect(m_colorPicker, SIGNAL(colorPicked(QColor)), this, SIGNAL(colorSelected(QColor)));

    resize(width(),ui->menuBox->height()+51);
    openedGroup = NULL;
}

ContextMenu::~ContextMenu()
{
    delete ui;
}

void ContextMenu::on_close_clicked()
{
    hideContextMenu();
}

void ContextMenu::hideContextMenu()
{
    m_colorPicker->hide();
    hide();
}

void ContextMenu::on_penwidth_toggled(bool checked)
{
    hideSubmenus();
    ui->penWidthBox->setVisible(checked);
    if(checked) {
        openedGroup = ui->penwidth;
        resizeToMin();
    }
}

void ContextMenu::on_shape_toggled(bool checked)
{
    hideSubmenus();
    ui->shapeBox->setVisible(checked);
    if(checked) {
        openedGroup = ui->shape;
        resizeToMin();
    }
}

void ContextMenu::on_color_clicked()
{
    /*hideSubmenus();
    ui->colorBox->setVisible(checked);
    if(checked) {
        openedGroup = ui->color;
        resizeToMin();
    }*/
    if(m_colorPicker->isHidden()) {
        QPoint p = geometry().center();
        m_colorPicker->move(p - QPoint(m_colorPicker->width()/2, m_colorPicker->height()/2) + QPoint(-30,0));
        m_colorPicker->show();
        m_colorPicker->raise();
    }
    else
        m_colorPicker->hide();
}

/*void ContextMenu::on_wt_toggled(bool checked)
{
    hideSubmenus();
    ui->wtBox->setVisible(checked);
    if(checked) {
        openedGroup = ui->wt;
        resizeToMin();
    }
}*/


void ContextMenu::hideSubmenus()
{
    if(openedGroup) {
        openedGroup->setChecked(false);
        openedGroup = NULL;
    }
}

void ContextMenu::resizeToMin()
{
    resize(width(), ui->menuBox->height() + 51);
    //move(pos()-QPoint(0,51));
}

void ContextMenu::on_open_clicked()
{
    emit open();
}

void ContextMenu::on_save_clicked()
{
    emit save();
}

void ContextMenu::on_sketch_clicked()
{
    emit sketch();
}

void ContextMenu::on_scrnshot_clicked()
{
    emit screenshot();
}

void ContextMenu::on_print_clicked()
{
    emit print();
}

void ContextMenu::on_penWidthDec_clicked()
{
    emit penWidthDecrease();
}

void ContextMenu::on_penWidthInc_clicked()
{
    emit penWidthIncrease();
}

void ContextMenu::on_rect_clicked()
{
    emit toolSelected(DRAWINGMODE_RECTANGLE);
}

void ContextMenu::on_ellipse_clicked()
{
    emit toolSelected(DRAWINGMODE_ELLIPSE);
}

void ContextMenu::on_white_clicked()
{
    emit colorSelected(Qt::white);
}

void ContextMenu::on_black_clicked()
{
    emit colorSelected(Qt::black);
}

void ContextMenu::on_red_clicked()
{
    emit colorSelected(Qt::red);
}

void ContextMenu::on_blue_clicked()
{
    emit colorSelected(Qt::blue);
}

void ContextMenu::on_pen_clicked()
{
    emit toolSelected(DRAWINGMODE_FREEHAND);
}

void ContextMenu::on_arrow_clicked()
{
    emit toolSelected(DRAWINGMODE_STRAIGHTLINE);
}

void ContextMenu::on_redo_clicked()
{
    emit redo();
}

void ContextMenu::on_undo_clicked()
{
    emit undo();
}

void ContextMenu::on_wt_clicked()
{
    emit print();
}

void ContextMenu::on_emptyRect_clicked()
{
    emit toolSelected(DRAWINGMODE_EMPTYRECTANGLE);
}

void ContextMenu::on_emptyEllipse_clicked()
{
    emit toolSelected(DRAWINGMODE_EMPTYELLIPSE);
}

void ContextMenu::on_eraser_toggled(bool checked)
{
    hideSubmenus();
    ui->wtBox->setVisible(checked);
    if(checked) {
        openedGroup = ui->wt;
        resizeToMin();
    }
}

void ContextMenu::on_erasePage_clicked()
{
    if(displayYesNoMessage("Remove all annotations on this page? \n(This can be reverted via undo)"))
        emit erasePage();
}

void ContextMenu::on_eraseAll_clicked()
{
    if(displayYesNoMessage("Remove all annotations in this content? \nThis cannot be undone!"))
        emit eraseAll();
}

void ContextMenu::on_eraseSmall_clicked()
{
    emit eraserSize(10);
    emit toolSelected(DRAWINGMODE_ERASER);
}

void ContextMenu::on_eraseBig_clicked()
{
    emit eraserSize(50);
    emit toolSelected(DRAWINGMODE_ERASER);
}
