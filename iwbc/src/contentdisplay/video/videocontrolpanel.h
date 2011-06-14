/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

#ifndef VIDEOCONTROLPANEL_H
#define VIDEOCONTROLPANEL_H

#include <QWidget>

namespace Ui {
    class VideoControlPanel;
}

class VideoControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit VideoControlPanel(QWidget *parent = 0);
    ~VideoControlPanel();

private:
    Ui::VideoControlPanel *ui;

signals:
    void playClicked();
    void pauseClicked();
    void stopClicked();
    void timelineChanged(float pos);

private slots:
    void on_stopButton_clicked();
    void on_pauseButton_clicked();
    void on_playButton_clicked();

    void on_timeline_sliderPressed();

public slots:
    void mediaLengthChanged(float length);
    void mediaPosChanged(float pos);
    void on_timeline_sliderReleased();
    void on_timeline_sliderMoved(int position);
};

#endif // VIDEOCONTROLPANEL_H
