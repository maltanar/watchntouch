#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include "appglobals.h"
#include <QWidget>
#include <QPushButton>


namespace Ui {
    class ContextMenu;
}

class ContextMenu : public QWidget
{
    Q_OBJECT

public:
    explicit ContextMenu(QWidget *parent = 0);
    ~ContextMenu();

private:
    Ui::ContextMenu *ui;
    void hideSubmenus();
    void resizeToMin();
    QPushButton *openedGroup;

private slots:
    void on_undo_clicked();
    void on_redo_clicked();
    void on_arrow_clicked();
    void on_eraser_clicked();
    void on_pen_clicked();
    void on_blue_clicked();
    void on_red_clicked();
    void on_black_clicked();
    void on_white_clicked();
    void on_ellipse_clicked();
    void on_rect_clicked();
    void on_penWidthInc_clicked();
    void on_penWidthDec_clicked();
    void on_print_clicked();
    void on_save_clicked();
    void on_open_clicked();
    void on_sketch_clicked();
    void on_scrnshot_clicked();
    void on_wt_toggled(bool checked);
    void on_color_toggled(bool checked);
    void on_shape_toggled(bool checked);
    void on_penwidth_toggled(bool checked);
    void on_close_clicked();

signals:
    void toolSelected(DrawingMode tool);
    void colorSelected(QColor color);
    void penWidthIncrease();
    void penWidthDecrease();
    void open();
    void save();
    void sketch();
    void screenshot();
    void print();
    void undo();
    void redo();
};

#endif // CONTEXTMENU_H
