#include "webcontrolpanel.h"
#include "ui_webcontrolpanel.h"

#include <QUrl>

WebControlPanel::WebControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WebControlPanel)
{
    ui->setupUi(this);
    // hide status area by default
    ui->statusArea->hide();
}

WebControlPanel::~WebControlPanel()
{
    delete ui;
}

void WebControlPanel::on_confirmUrl_clicked()
{
    emit locationChanged(QUrl::fromUserInput(ui->urlEdit->text()));
}

void WebControlPanel::loadStarted()
{
    ui->statusArea->show();
    ui->progressBar->setValue(0);
    ui->statusLabel->setText("Loading web page, please wait...");
}

void WebControlPanel::loadFinished(bool ok)
{
    if(ok)
        ui->statusArea->hide();
    else
        ui->statusLabel->setText("Failed to load requested web page");
}

void WebControlPanel::loadProgress(int progress)
{
    ui->progressBar->setValue(progress);
}

void WebControlPanel::loadedPageChanged(QUrl newLocation)
{
    ui->urlEdit->setText(newLocation.toString());
}

void WebControlPanel::on_annotationEnabled_toggled(bool checked)
{
    emit requestReadOnly(!checked);
}
