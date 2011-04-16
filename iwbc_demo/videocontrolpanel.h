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

private slots:
    void on_stopButton_clicked();
    void on_pauseButton_clicked();
    void on_playButton_clicked();
};

#endif // VIDEOCONTROLPANEL_H
