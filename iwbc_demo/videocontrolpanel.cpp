#include "videocontrolpanel.h"
#include "ui_videocontrolpanel.h"

VideoControlPanel::VideoControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoControlPanel)
{
    ui->setupUi(this);
}

VideoControlPanel::~VideoControlPanel()
{
    delete ui;
}

void VideoControlPanel::on_playButton_clicked()
{
    emit playClicked();
}

void VideoControlPanel::on_pauseButton_clicked()
{
    emit pauseClicked();
}

void VideoControlPanel::on_stopButton_clicked()
{
    emit stopClicked();
}
