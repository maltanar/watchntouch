#include "webcontrolpanel.h"
#include "ui_webcontrolpanel.h"

#include <QUrl>

WebControlPanel::WebControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WebControlPanel)
{
    ui->setupUi(this);
}

WebControlPanel::~WebControlPanel()
{
    delete ui;
}

void WebControlPanel::on_confirmUrl_clicked()
{
    emit locationChanged(QUrl::fromUserInput(ui->urlEdit->text()));
}
