#include <QDebug>
#include <QRegExp>
#include <QEventLoop>
#include <QTimer>
#include <QSettings>
#include <QFile>
#include "printer.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;

Printer::Printer(QObject *parent) :
    QObject(parent)
{

    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    hand_ip = p->value("server_ip").toString();
    p->endGroup();
    gcode_cmd = new QNetworkAccessManager(this);

    QObject::connect(gcode_cmd, SIGNAL(finished(QNetworkReply*)), this, SLOT(CmdFinishedSlot(QNetworkReply*)));

}

Printer::~Printer()
{

}

void Printer::Delay(int mesc)
{
    QEventLoop eventloop;
    QTimer::singleShot(mesc,&eventloop,SLOT(quit()));
    eventloop.exec();
}

void Printer::CmdFinishedSlot(QNetworkReply *reply)
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

void Printer::TakePhoto()
{
//    cv::Mat src;
//    QFile::remove("img.jpg");
//    QCameraer::getInstance().CapatureOnce(src);
//    QCameraer::getInstance().CapatureOnce(src);
//    std::string file("img.jpg");
//    cv::imwrite(file,src);
}

void Printer::ConnectPort()
{
//    QUrl url_cmd(tr("http://%1:5000/%2").arg(hand_ip).arg(QString("connect/j")));
//    gcode_cmd->get(QNetworkRequest(url_cmd));
//    serail_port->ConnectPort("/dev/ttyAMA0");
    CPSerialPort::getInstance().ConnectPort("/dev/ttyAMA0");
}

void Printer::DisConnect()
{
    QUrl url_cmd(tr("http://%1:5000/%2").arg(hand_ip).arg(QString("disconnect")));
    gcode_cmd->get(QNetworkRequest(url_cmd));
    CPSerialPort::getInstance().Disconnect();
}

void Printer::LoadGcode(QString filename)
{
    file_name = filename;
}

void Printer::SendGcode(QString cmd)
{
//    QUrl url_cmd(tr("http://%1:5000/sendcmd/%2").arg(hand_ip).arg(cmd));
//    gcode_cmd->get(QNetworkRequest(url_cmd));
//    serail_port->WritePort(cmd);
    CPSerialPort::getInstance().WritePort(cmd);
}

void Printer::WritePort(QString cmd)
{

}

void Printer::SetStatus(status s_t)
{

}

int Printer::GetPercent()
{
    QUrl url_cmd(tr("http://%1:5000/%2").arg(hand_ip).arg(QString("getprogress")));
    gcode_cmd->get(QNetworkRequest(url_cmd));
}

QStringList Printer::GetInfo()
{
    QUrl url_cmd(tr("http://%1:5000/%2").arg(hand_ip).arg(QString("gettemp")));
    gcode_cmd->get(QNetworkRequest(url_cmd));
}

void Printer::StartPrint()
{
    QUrl url_cmd(tr("http://%1:5000/startprint/%2").arg(hand_ip).arg(file_name));
    gcode_cmd->get(QNetworkRequest(url_cmd));
}

void Printer::PausePrint()
{
    QUrl url_cmd(tr("http://%1:5000/%2").arg(hand_ip).arg(QString("pauseprint")));
    gcode_cmd->get(QNetworkRequest(url_cmd));
}

void Printer::RecoverPrint()
{
    QUrl url_cmd(tr("http://%1:5000/%2").arg(hand_ip).arg(QString("recoverprint")));
    gcode_cmd->get(QNetworkRequest(url_cmd));
}

void Printer::CanclePrint()
{
    QUrl url_cmd(tr("http://%1:5000/%2").arg(hand_ip).arg(QString("pauseprint")));
    gcode_cmd->get(QNetworkRequest(url_cmd));

    QUrl url_cmd_reset(tr("http://%1:5000/%2").arg(hand_ip).arg(QString("reset")));
    gcode_cmd->get(QNetworkRequest(url_cmd_reset));
}
