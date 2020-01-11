#ifndef MSERVER_H
#define MSERVER_H

#include <QTcpServer>
#include <QUdpSocket>
#include "serverthread.h"

class MServer : public QTcpServer
{
    Q_OBJECT
public:
    int totalClient;  //客户端数
    explicit MServer(QObject *parent = 0);
    QString GetIp();

public slots:
    void OutConnect();
    void SlotCmd(QString);
    void SlotUploadSuccess(QString filename);
    void SlotNConnected(bool);
    void SlotNewConnection();
    void SlotPengingDatagram();

protected:
    void incomingConnection(int socketDescriptor);  //虚函数，有tcp请求时会触发

private:
    ServerThread* thread;

    QUdpSocket * receiver;



signals:
    void SigConnect(bool);
    void SigUpload(QString);
    void SigCmd(QString);
};

#endif // MSERVER_H
