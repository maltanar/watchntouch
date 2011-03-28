#ifndef CONTENTSELECTOR_H
#define CONTENTSELECTOR_H
#include "appglobals.h"
#include <QDialog>
#include <QLabel>
#include "recentlyused.h"


namespace Ui {
    class ContentSelector;
}

class ContentSelector : public QDialog
{
    Q_OBJECT

public:
    explicit ContentSelector(QWidget *parent = 0);
    ~ContentSelector();

    QString getSelectedContent();

private:
    Ui::ContentSelector *ui;
    QLabel *recentItem[NUM_RECENT_ITEMS];
    QString selectedContent;

    void loadRecentlyUsedList();
    void selectContent(QString content);
    void openGoogleDoc(QString googleDocId);

public slots:
    void recentItemClicked(QString url);

private slots:
    void on_takeScreenshot_clicked();
    void on_openSketch_clicked();
    void on_newSketch_clicked();
    void on_openGDoc_clicked();
    void on_login_clicked();
    void on_cancelButton_clicked();
    void on_browseButton_clicked();
};

#endif // CONTENTSELECTOR_H
