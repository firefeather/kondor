#include <QFile>
#include <QProcess>
#include <QSettings>
#include "wifipwd.h"
#include "ui_wifipwd.h"

WifiPWD::WifiPWD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiPWD)
{
    ui->setupUi(this);
}

WifiPWD::~WifiPWD()
{
    delete ui;
}

void WifiPWD::on_btnBack_clicked()
{
    this->close();
}

void WifiPWD::on_btnWifiOK_clicked()
{
    QString pwd = ui->lineEdit->text();
    if(!pwd.isEmpty())
    {
        QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
        p->beginGroup("Config");
        p->setValue("current_pwd",pwd);
#ifdef Q_OS_LINUX
        QString ssid = p->value("current_ssid").toString();
#elif defined (Q_OS_WIN32)
        QString ssid = "binro";
#endif
        p->endGroup();
        delete  p;
        QFile::remove("wpa_supplicant.conf");
        QString files_str = tr("country=CN \nctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev \nupdate_config=1\n");
        QString network = files_str + tr("network={\n    ssid=\"%1\"\n    psk=\"%2\"\n    key_mgmt=WPA-PSK\n    priority=1\n}").arg(ssid).arg(pwd);
        QFile f("wpa_supplicant.conf");
        f.open(QIODevice::ReadWrite);
        f.write(network.toLatin1());
        f.close();

#ifdef Q_OS_LINUX
        QProcess process;
        QStringList options;
        options<<"-c";
        options<<"sudo cp ./wpa_supplicant.conf /boot/";
        process.start("/bin/bash", options);
        process.waitForFinished(-1);

        options.clear();
        options<<"-c";
        options<<"sudo cp ./wpa_supplicant.conf /etc/wpa_supplicant/";
        process.start("/bin/bash", options);
        process.waitForFinished(-1);

        options.clear();
        options<<"-c";
        options<<"sudo wpa_cli -i wlan0 reconfigure";
        process.start("/bin/bash", options);
        process.waitForFinished(-1);

        //        this->close();
#endif
    }

}
