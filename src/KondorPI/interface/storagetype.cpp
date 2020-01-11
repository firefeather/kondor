#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QFileInfo>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "storagetype.h"
#include "ui_storagetype.h"

StorageType::StorageType(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StorageType)
{
    ui->setupUi(this);
    setWindowFlags (Qt::FramelessWindowHint);
    ui->treeWidget->setStyleSheet("QTreeWidget { font-size: 16pt; }");
    UpdateTree();

}

StorageType::~StorageType()
{
    delete ui;
}

void StorageType::on_btnWifiOK_clicked()
{
    if(ui->treeWidget->currentItem()!=NULL)
    {
        QString name = ui->treeWidget->currentItem()->text(0);
        if(!name.isEmpty())
        {
            QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
            p->beginGroup("Config");
            p->setValue("current_file",name);
            p->endGroup();
            delete  p;
            this->close();
            emit SigPreview();
        }
    }


}

void StorageType::UpdateTree()
{
    ui->treeWidget->clear();
    QDir* dir = new QDir("./");
    QStringList filter;
    filter<<"*.slc"<<"*.bgz";
    dir->setNameFilters(filter);
    QList<QFileInfo>* file_infos = new QList<QFileInfo>(dir->entryInfoList(filter));
    ui->treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;

    ui->treeWidget->insertTopLevelItems(0, items);

    for(int ts=0;ts<file_infos->count();ts++)
    {
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(file_infos->at(ts).fileName())));
    }
    if(items.size()>0)
        ui->treeWidget->insertTopLevelItems(0, items);
}

void StorageType::on_btnBack_clicked()
{
    UpdateTree();
    this->close();
}
//删除选中的本地文件
void StorageType::on_btnDelete_clicked()
{
    if(ui->treeWidget->currentItem()!=NULL)
    {
        QString name = ui->treeWidget->currentItem()->text(0);
        QFile::remove(name);
        UpdateTree();
    }

}
