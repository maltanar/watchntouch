/**************************************************************************************
**
** Copyright (C) 2011, Yaman Umuroglu, Utku Sirin, Giray Havur, Melike Ercan.
** All rights reserved.
** This program is distributed under the terms of the GNU Lesser General Public License
**
***************************************************************************************/

#ifndef SESSIONJOINDIALOG_H
#define SESSIONJOINDIALOG_H

#include <QDialog>
#include "collaborationclient.h"

namespace Ui {
    class SessionJoinDialog;
}

class SessionJoinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SessionJoinDialog(CollaborationClient * client, QWidget *parent = 0);
    ~SessionJoinDialog();

private:
    Ui::SessionJoinDialog *ui;
    CollaborationClient *m_client;

private slots:
    void on_btn_createSession_clicked();
    void on_joinSession_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_sessionList_doubleClicked(QModelIndex index);
    void on_pushButton_clicked();
    void on_refreshList_clicked();
    void foundServer(QHostAddress addr, QString serverName);
    void loginResult(bool result, QString infoMsg);
    void gotSessionList(QStringList sessionList);
};

#endif // SESSIONJOINDIALOG_H
