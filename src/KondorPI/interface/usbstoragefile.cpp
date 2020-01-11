#include <QDir>
#include <QProcess>
#include <QSettings>
#include <QFileInfo>
#include "usbstoragefile.h"
#include "ui_usbstoragefile.h"

USBStorageFile::USBStorageFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::USBStorageFile)
{
    ui->setupUi(this);
    ui->treeWidget->setStyleSheet("QTreeWidget { font-size: 16pt; }");
}

USBStorageFile::~USBStorageFile()
{
    delete ui;
}

void USBStorageFile::on_btnBack_clicked()
{
    this->close();
}

void USBStorageFile::UpdateTree()
{
    ui->treeWidget->clear();

    QProcess process;
    QStringList options;
    options<<"-c";
    options<<"ls /media/pi";
    process.start("/bin/bash", options);
    process.waitForFinished(-1);
    stdout_str = "/media/pi/"+process.readAllStandardOutput();

    QDir* dir = new QDir(stdout_str.remove("\n"));
    QStringList filter;
    filter<<"*.slc"<<"*.bgz";
    dir->setNameFilters(filter);
    QList<QFileInfo>* file_infos = new QList<QFileInfo>(dir->entryInfoList(filter));
    ui->treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;

//    ui->treeWidget->insertTopLevelItems(0, items);

    for(int ts=0;ts<file_infos->count();ts++)
    {
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(file_infos->at(ts).fileName())));
    }
    if(items.size()>0)
        ui->treeWidget->insertTopLevelItems(0, items);
}

//选择对应的文件
void USBStorageFile::on_btnWifiOK_clicked()
{

    if(ui->treeWidget->currentItem()!=NULL)
    {
        QString name = ui->treeWidget->currentItem()->text(0);
        if(!name.isEmpty())
        {
            QString files = stdout_str.remove("\n")+"/"+name;
            QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
            p->beginGroup("Config");
            p->setValue("current_file",files);
            p->endGroup();
            delete  p;
            this->close();
            emit SigPreview();
        }
    }
}
