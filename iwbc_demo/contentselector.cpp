#include "contentselector.h"
#include "ui_contentselector.h"

#include <QVBoxLayout>
#include <QFileDialog>

ContentSelector::ContentSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContentSelector)
{
    ui->setupUi(this);

    ui->gdocsBox->setVisible(false);

    selectedContent = "";

    // create the display labels we use for the recent items
    QVBoxLayout *layout = new QVBoxLayout();

    for(int i=0; i < NUM_RECENT_ITEMS; i++) {
        recentItem[i] = new QLabel(this);
        recentItem[i]->setText("");   // all items empty by default
        connect(recentItem[i], SIGNAL(linkActivated(QString)), this, SLOT(recentItemClicked(QString)));
        layout->addWidget(recentItem[i]);
    }
    ui->recentBox->setLayout(layout);

    loadRecentlyUsedList();
}

ContentSelector::~ContentSelector()
{
    delete ui;
}

void ContentSelector::loadRecentlyUsedList()
{
    if(recentlyUsed->size() == 0) {
        // no items in the recently used list
        recentItem[0]->setText("(no files in list)");
    }
    // fill the contents of the recently used item labels
    QString title, url;
    QString style = "text-decoration:none; color:black;";
    for(int i=0; i < recentlyUsed->size(); i++) {
        recentlyUsed->getRecentItem(i, title, url);
        recentItem[i]->setToolTip(url);
        recentItem[i]->setText(QString("<a href='%1' style='%2'><b>&gt; %3</b><br>%4</a>").arg(url,style,title,url));
    }
}


void ContentSelector::recentItemClicked(QString url)
{
    // TODO what to do for Google Docs?
    selectContent(url);
}

QString ContentSelector::getSelectedContent()
{
    return selectedContent;
}

void ContentSelector::on_browseButton_clicked()
{
    // TODO what to do for filters for a generic case?
    QString fileName = QFileDialog::getOpenFileName(this, "Select content", "","PDF files (*.pdf)");
    selectContent(fileName);
}

void ContentSelector::selectContent(QString content)
{
    if(content != "") {
        selectedContent = content;
        accept();
    } else
        reject();
}

void ContentSelector::on_cancelButton_clicked()
{
    selectContent("");
}

void ContentSelector::on_login_clicked()
{
    if(googleDocsAccess->login(ui->username->text(), ui->password->text())) {
        ui->statusMsg->setText("Login succeeded!");
        googleDocsAccess->fetchList();
        QList<GoogleDocEntry> docList = googleDocsAccess->getDocumentList();

        QListWidgetItem *currentItem;
        for(int i=0; i < docList.size(); i++) {
            currentItem = new QListWidgetItem(docList.at(i).documentTitle);
            currentItem->setData(Qt::UserRole, QVariant(docList.at(i).resourceId));
            ui->presentatonList->addItem(currentItem);
        }

        ui->presentatonList->setVisible(true);
    } else
        ui->statusMsg->setText("Login failed!");
}
