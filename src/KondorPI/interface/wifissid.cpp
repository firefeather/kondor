#include <QDebug>
#include <QProcess>
#include <QSettings>
#include "wifissid.h"
#include "ui_wifissid.h"

WifiSSID::WifiSSID(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiSSID)
{
    ui->setupUi(this);
    ui->treeWidget->setStyleSheet("QTreeWidget { font-size: 16pt; }");
    pwd = new WifiPWD();
}

WifiSSID::~WifiSSID()
{
    delete ui;
}

void WifiSSID::on_btnBack_clicked()
{
    this->close();
}

void WifiSSID::on_btnWifiOK_clicked()
{
    QString name = ui->treeWidget->currentItem()->text(0);
    if(!name.isEmpty())
    {
        QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
        p->beginGroup("Config");
        p->setValue("current_ssid",name);
        p->endGroup();
        delete  p;
        this->close();
    }

#ifdef Q_OS_LINUX
    pwd->showFullScreen();
#elif defined (Q_OS_WIN32)
    pwd->showNormal();
#endif
}

void WifiSSID::on_btnScan_clicked()
{
    ui->treeWidget->clear();
#ifdef Q_OS_LINUX
    QProcess process;
    QStringList options;
    options<<"-c";
    options<<"sudo iwlist wlan0 scan | grep SSID";
    process.start("/bin/bash", options);
    process.waitForFinished(-1);
    QString stdout_str = process.readAllStandardOutput();


    qDebug()<<"get result:"<<stdout_str;

    QString str_output = stdout_str;
    QStringList wifi_list;

    QRegExp reg("SSID:\"([\\s\\S]*)\"");
    reg.setMinimal(true);
    int pos = 0;
    if(str_output.contains(reg))
    {
        while((pos=reg.indexIn(str_output,pos)) != -1)
        {
            qDebug()<<"get ssid:"<<reg.cap(1);
            wifi_list<<reg.cap(1);
            pos += reg.matchedLength();
        }
    }

    QList<QTreeWidgetItem *> items;
    ui->treeWidget->insertTopLevelItems(0, items);

    for(int ts=0;ts<wifi_list.size();ts++)
    {
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(wifi_list.at(ts))));
    }
    ui->treeWidget->insertTopLevelItems(0, items);
#elif defined (Q_OS_WIN32)
    pwd->showNormal();
#endif
}

