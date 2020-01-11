#include <QHostInfo>
#include <QSettings>
#include <QNetworkInterface>
#include "mserver.h"

#include "tools/easylogging++.h"

#define GET_HOST_COMMAND "Kondor3D"

MServer::MServer(QObject *parent) :
    QTcpServer(parent)
{
    totalClient = 0;
    this->listen(QHostAddress::Any,6666);
    connect(this,SIGNAL(newConnection()),this,SLOT(SlotNewConnection()));

    receiver = new QUdpSocket(this);
    receiver->bind(6667,QUdpSocket::ShareAddress);
    //readyRead:每当有数据报来时发送这个信号
    connect(receiver,SIGNAL(readyRead()),this,SLOT(SlotPengingDatagram()));

}

void MServer::incomingConnection(int socketDescriptor)
{

    if(totalClient>=200)
    {
        QTcpSocket* temp = new QTcpSocket();
        temp->setSocketDescriptor(socketDescriptor);
        QString send = "NOO";
        temp->write(send.toLatin1());
        qDebug()<<"Print is working!";
        //        delete temp;
    }
    else
    {
        ServerThread *thread = new ServerThread(this,socketDescriptor);
        QObject::connect(thread,SIGNAL(out()),this,SLOT(OutConnect()));
        QObject::connect(thread,SIGNAL(SigCmd(QString)),this,SLOT(SlotCmd(QString)));
        QObject::connect(thread,SIGNAL(SigLoaded(QString)),this,SLOT(SlotUploadSuccess(QString)));
        QObject::connect(thread,SIGNAL(SigConnected(bool)),this,SLOT(SlotNConnected(bool)));

        thread->start();
        totalClient++;
        LOG(DEBUG)<<"comming connect!";
    }
}
void MServer::OutConnect()
{
    totalClient--;
    delete thread;
}
void MServer::SlotPengingDatagram()
{
    QHostAddress client_address;//声明一个QHostAddress对象用于保存发送端的信息
    //char buf[100];//声明一个字符数组用于接收发送过来的字符串
    //数据报不为空
    while( receiver->hasPendingDatagrams() )
    {
        quint16 recPort = 0;
        QByteArray datagram;
        //datagram大小为等待处理数据报的大小才能就收数据;
        datagram.resize( receiver->pendingDatagramSize() );
        //接收数据报
        receiver->readDatagram(datagram.data(),datagram.size(), &client_address, &recPort);

        QString strData= datagram;
        int ret = strData.compare(GET_HOST_COMMAND);
        LOG(DEBUG)<<strData.toStdString();
        if (0 == ret)
        {
            QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
            p->beginGroup("Config");
            QString name = p->value("DeviceName").toString();
            p->endGroup();
            delete  p;

            QByteArray datagback = name.toLatin1() + ":"+GetIp().toLatin1();
            receiver->writeDatagram(datagback,datagback.size(),client_address,recPort);
        }
    }

}
QString MServer::GetIp()
{
//        QString localhostname =  QHostInfo::localHostName();
//          QString localhostIP;
    //      QList<QHostAddress> hostList = QHostInfo::fromName(localhostname).addresses();
    //      foreach (const QHostAddress& address, hostList)
    //      {
    //          if ((address.protocol() == QAbstractSocket::IPv4Protocol) && (address != QHostAddress(QHostAddress::LocalHost)))
    //          {
    //               localhostIP = address.toString();
    //               return localhostIP;
    //          }
    //      }
    //    return 0;


    QString localhostIP;
    foreach(QNetworkInterface intf, QNetworkInterface::allInterfaces())
    {
        if(intf.flags().testFlag(QNetworkInterface::IsUp) && !intf.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            foreach (QNetworkAddressEntry entry, intf.addressEntries())
            {
                if(intf.hardwareAddress() != "00:00:00:00:00:00" && entry.ip().toString().contains(".") && intf.humanReadableName().contains("wlan"))
                {
                    localhostIP = entry.ip().toString();

                    LOG(DEBUG)<<"Entry ip :"<<localhostIP.toStdString();

                    return localhostIP;
                }

            }
        }

    }
    return 0;
}

void MServer::SlotCmd(QString log)
{
    qDebug()<<"get the client info:"<<log;
    emit SigCmd(log);
}
void MServer::SlotNConnected(bool connect)
{
    emit  SigConnect(connect);
}

void MServer::SlotNewConnection()
{
    emit  SigConnect(true);
}
void MServer::SlotUploadSuccess(QString filename)
{
    emit SigUpload(filename);
}
