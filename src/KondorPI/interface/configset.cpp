#include <QSettings>

#include "configset.h"
#include "ui_configset.h"

ConfigSet::ConfigSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigSet)
{
    ui->setupUi(this);
    gcode_cmd = new QNetworkAccessManager(this);
    QObject::connect(gcode_cmd, SIGNAL(finished(QNetworkReply*)), this, SLOT(CmdFinishedSlot(QNetworkReply*)));

    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    ui->doubleSpinBoxThickness->setValue(p->value("normal_layer_thickness").toDouble());
    ui->spinBox->setValue(p->value("init_expose_time").toInt());
    ui->spinBoxCounts->setValue(p->value("init_layer_count").toInt());
    ui->spinBoxNormalTime->setValue(p->value("normal_expose_time").toInt());
    p->endGroup();
    delete  p;

    QNetworkInterface thisNet;
    QList<QNetworkInterface>NetList;
    NetList = QNetworkInterface::allInterfaces();
    int count = NetList.count();
    int net = 0;
    QList<QHostAddress> alladdress;
    for(net=0;net<count;net++)
    {
        if(NetList[net].name()=="wlan0")
        {
            alladdress =  NetList[net].allAddresses();
        }
    }
    foreach (QHostAddress s, alladdress)
    {
        if((s.protocol()==QAbstractSocket::IPv4Protocol)&&(s!=QHostAddress(QHostAddress::LocalHost)))
        {
            qDebug()<<s.toString();
            ui->labelIPAddress->setText(s.toString());
        }
    }

    ssid = new WifiSSID();

}

ConfigSet::~ConfigSet()
{
    delete ui;
    delete ssid;
}

void ConfigSet::CmdFinishedSlot(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"get reply error!";
    }
    else
    {
        QByteArray responseData = reply->readAll();
        qDebug()<<"Tmp:"<<responseData;

    }
}

void ConfigSet::on_btnBack_clicked()
{
    this->close();
}

//初始层曝光时间
void ConfigSet::on_spinBox_valueChanged(int arg1)
{
    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    p->setValue("init_expose_time",arg1);
    p->endGroup();
    delete  p;
}
//初始层层数
void ConfigSet::on_spinBoxCounts_valueChanged(int arg1)
{
    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    p->setValue("init_layer_count",arg1);
    p->endGroup();
    delete  p;
}
//正常曝光参数
void ConfigSet::on_spinBoxNormalTime_valueChanged(int arg1)
{
    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    p->setValue("normal_expose_time",arg1);
    p->endGroup();
    delete  p;
}

void ConfigSet::on_doubleSpinBoxThickness_valueChanged(double arg1)
{
    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    p->setValue("normal_layer_thickness",arg1);
    p->endGroup();
    delete  p;
}
//设置wifi
void ConfigSet::on_btnSetWifi_clicked()
{
#ifdef Q_OS_LINUX
    ssid->showFullScreen();
#elif defined (Q_OS_WIN32)
    ssid->showNormal();
#endif
}
