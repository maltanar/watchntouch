#include "contextmenu.h"
#include "ui_contextmenu.h"

ContextMenu::ContextMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContextMenu)
{
    ui->setupUi(this);

    ui->shapeBox->hide();
    ui->colorBox->hide();
    ui->wtBox->hide();
    ui->penWidthBox->hide();

    resize(width(),ui->menuBox->height()+51);
    openedGroup = NULL;
}

ContextMenu::~ContextMenu()
{
    delete ui;
}

void ContextMenu::on_close_clicked()
{
    hide();
}

void ContextMenu::on_penwidth_toggled(bool checked)
{
    ui->penWidthBox->setVisible(checked);
    if(checked) {
        hideSubmenus();
        openedGroup = ui->penwidth;
        resizeToMin();
    }
}

void ContextMenu::on_shape_toggled(bool checked)
{
    ui->shapeBox->setVisible(checked);
    if(checked) {
        hideSubmenus();
        openedGroup = ui->shape;
        resizeToMin();
    }
}

void ContextMenu::on_color_toggled(bool checked)
{
    ui->colorBox->setVisible(checked);
    if(checked) {
        hideSubmenus();
        openedGroup = ui->color;
        resizeToMin();
    }
}

void ContextMenu::on_wt_toggled(bool checked)
{
    ui->wtBox->setVisible(checked);
    if(checked) {
        hideSubmenus();
        openedGroup = ui->wt;
        resizeToMin();
    }
}


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

void ContextMenu::on_eraser_clicked()
{
    emit toolSelected(DRAWINGMODE_ERASER);
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
