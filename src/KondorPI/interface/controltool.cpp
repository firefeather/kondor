#include <QSettings>
#include <QProcess>
#include <QTimer>

#include "printer/hardware/serialport/cpserialport.h"
#include "controltool.h"
#include "ui_controltool.h"

ControlTool::ControlTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlTool)
{
    ui->setupUi(this);
    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    hand_ip = p->value("server_ip").toString();
    hand_ip = "127.0.0.1";
    p->endGroup();
    delete p;

    gcode_cmd = new QNetworkAccessManager(this);
    QObject::connect(gcode_cmd, SIGNAL(finished(QNetworkReply*)), this, SLOT(CmdFinishedSlot(QNetworkReply*)));
    step_unit = 1;
    z_height = 0;
    b_lcd_light = false;
    b_fan_on = false;

    QTimer::singleShot((3000),this,SLOT(SlotAutoConnect()));
}

ControlTool::~ControlTool()
{
    delete ui;
}
void ControlTool::SlotAutoConnect()
{
    QUrl url_cmd(tr("http://%1:5000/connect/j").arg(hand_ip));
    gcode_cmd->get(QNetworkRequest(url_cmd));
    CPSerialPort::getInstance().ConnectPort("/dev/ttyAMA0");

    CPSerialPort::getInstance().WritePort("$X");
    CPSerialPort::getInstance().WritePort("G90");
    CPSerialPort::getInstance().WritePort("F300");
    CPSerialPort::getInstance().WritePort("$H");
    CPSerialPort::getInstance().WritePort("G4 P0.1");
}

void ControlTool::CmdFinishedSlot(QNetworkReply *reply)
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

void ControlTool::SendCmd(QString cmd)
{
//    QUrl url_cmd(tr("http://%1:5000/sendcmd/%2").arg(hand_ip).arg(cmd));
//    gcode_cmd->get(QNetworkRequest(url_cmd));
    CPSerialPort::getInstance().WritePort(cmd);
}
void ControlTool::on_btnBack_clicked()
{
    this->close();
}

//1mm
void ControlTool::on_radioButton1_clicked()
{
    step_unit = 0.1;
}

void ControlTool::on_radioButton10_clicked()
{
    step_unit = 1.0;
}
//100mm
void ControlTool::on_radioButton100_clicked()
{
    step_unit = 10;
}

void ControlTool::on_z_add_clicked()
{
    z_height += step_unit;
    SendCmd(tr("G91"));
    SendCmd(tr("G1 Z-%1 F300").arg(step_unit));
    SendCmd(tr("G90"));
}

void ControlTool::on_z_sub_clicked()
{
    z_height -= step_unit;
    SendCmd(tr("G91"));
    SendCmd(tr("G1 Z+%1 F300").arg(step_unit));
    SendCmd(tr("G90"));
}

//复位
void ControlTool::on_Home_clicked()
{
    z_height = 0;
    SendCmd(tr("$H"));
}

//移动到底端
void ControlTool::on_btnBottom_clicked()
{
    SendCmd(QString("$H"));
    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    double z_height = p->value("z_height").toDouble();
    p->endGroup();
    delete p;

    SendCmd(tr("G92 X0 Y0 Z%1").arg(z_height));
    SendCmd(tr("G1 Z0 F300"));

}

//设置Z轴行程
void ControlTool::on_btnZMax_clicked()
{
    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    p->setValue("z_height",z_height);
    p->endGroup();
    delete p;
}
//LCD ON/OFF
void ControlTool::on_btnLCDOn_clicked()
{
    if(b_lcd_light)
    {
        //灯灭了
        b_lcd_light = false;
        ui->btnLCDOn->setText(QString("LCD ON"));
        SendCmd(QString("M3"));
    }
    else
    {
        //灯亮了
        b_lcd_light = true;
        ui->btnLCDOn->setText(QString("LCD OFF"));
        SendCmd(QString("M5"));
    }

}
//LCD Gird
void ControlTool::on_btnGird_clicked()
{
    QImage gird_img("./lcd_gird.jpg");
    Projector::getInstance().showImage(gird_img);
}

//重新连接
void ControlTool::on_btnReset_clicked()
{
    QUrl url_cmd0(tr("http://%1:5000/disconnect").arg(hand_ip));
    QUrl url_cmd(tr("http://%1:5000/connect/j").arg(hand_ip));
    gcode_cmd->get(QNetworkRequest(url_cmd0));
    gcode_cmd->get(QNetworkRequest(url_cmd));
}
//开启风扇
void ControlTool::on_btnFanOn_clicked()
{
//b_fan_on
    if(b_fan_on)
    {
        ui->btnFanOn->setText("FanOn");
        b_fan_on = false;
        SendCmd(QString("M8"));
    }
    else
    {
        ui->btnFanOn->setText("FanOff");
        b_fan_on = true;
        SendCmd(QString("M9"));

    }
}
//重启
void ControlTool::on_btnReboot_clicked()
{
    QProcess process;
    QStringList options;
    options<<"-c";
    options<<"sudo reboot";
    process.start("/bin/bash", options);
    process.waitForFinished(-1);
}
