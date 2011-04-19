#ifndef WEBCONTROLPANEL_H
#define WEBCONTROLPANEL_H

#include <QWidget>
#include <QUrl>

namespace Ui {
    class WebControlPanel;
}

class WebControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit WebControlPanel(QWidget *parent = 0);
    ~WebControlPanel();

private slots:
    void on_confirmUrl_clicked();

private:
    Ui::WebControlPanel *ui;

signals:
    void locationChanged(QUrl newLocation);
};

#endif // WEBCONTROLPANEL_H
