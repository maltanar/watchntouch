/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

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

void VideoControlPanel::on_timeline_sliderPressed()
{
    //emit pauseClicked();
}

void VideoControlPanel::on_timeline_sliderReleased()
{
    //emit playClicked();
}

void VideoControlPanel::on_timeline_sliderMoved(int position)
{
    qWarning() << "slider position" << position;
    emit timelineChanged(position * 1000);
}
