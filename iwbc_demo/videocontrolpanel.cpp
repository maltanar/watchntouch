#include "videocontrolpanel.h"
#include "ui_videocontrolpanel.h"

#include <QDebug>

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

void VideoControlPanel::mediaLengthChanged(float length)
{
    int iLen = length / 1000;
    ui->timeline->setMaximum(iLen);
}

void VideoControlPanel::mediaPosChanged(float pos)
{
    int iPos = pos / 1000;
    ui->timeline->setValue(iPos);
}
