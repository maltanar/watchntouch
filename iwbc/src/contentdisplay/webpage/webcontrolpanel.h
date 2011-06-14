/******************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU General Public License
**
*******************************************************************************/

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

public slots:
    void loadStarted();
    void loadFinished(bool ok);
    void loadProgress(int progress);
    void loadedPageChanged(QUrl newLocation);

private slots:
    void on_annotationEnabled_toggled(bool checked);
    void on_confirmUrl_clicked();

private:
    Ui::WebControlPanel *ui;

signals:
    void locationChanged(QUrl newLocation);
    void requestReadOnly(bool status);
};

#endif // WEBCONTROLPANEL_H
