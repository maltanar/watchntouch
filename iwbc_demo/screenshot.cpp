#include "screenshot.h"
#include "ui_screenshot.h"

Screenshot::Screenshot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Screenshot)
{
    ui->setupUi(this);
}

Screenshot::~Screenshot()
{
    delete ui;
}

void Screenshot::on_scrnshot_clicked()
{
    emit goBack();
}
