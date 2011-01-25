#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QWidget>

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
};

#endif // CONTEXTMENU_H
