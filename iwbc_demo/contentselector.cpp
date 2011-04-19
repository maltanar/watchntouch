#include "contentselector.h"
#include "ui_contentselector.h"

#include <QVBoxLayout>
#include <QFileDialog>

#include "googledocsaccess.h"

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
        recentItem[i]->setText(QString("<a href='%1' style='%2'><b>&gt; %3</b><br>%4</a>").arg(url,style,title,url.length() > 50 ? url.left(47)+"..." : url));
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
    QString fileName = QFileDialog::getOpenFileName(this, "Select content", "","Supported files (*.pdf *.ppt *.odp *.mp4 *.avi *.flv *.html *.htm)");
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
    // TODO logout and clear
    // TODO make status of open content dialog persistent?
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

        ui->gdocsBox->setVisible(true);
    } else
        ui->statusMsg->setText("Login failed!");
}

void ContentSelector::on_openGDoc_clicked()
{
    if(ui->presentatonList->currentItem())
        openGoogleDoc(ui->presentatonList->currentItem()->data(Qt::UserRole).toString());
}

void ContentSelector::openGoogleDoc(QString googleDocId)
{
    QString targetFileName =  CACHE_DIR + "/" + googleDocId + ".pdf";
    if(googleDocsAccess->downloadPresentation(googleDocId, targetFileName, "pdf"))
        selectContent(targetFileName);
    else
        displayErrorMessage("Could not open the Google document with the specified ID: \n" + googleDocId);
}

void ContentSelector::on_newSketch_clicked()
{
    selectContent("$newsketch$");
}

void ContentSelector::on_openSketch_clicked()
{
    selectContent("$existingsketch$");
}

void ContentSelector::on_takeScreenshot_clicked()
{
    selectContent("$screenshot$");
}

void ContentSelector::on_webSiteButton_clicked()
{
    selectContent("$webpage$");
}
