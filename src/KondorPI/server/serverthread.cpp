#include <QRegExp>
#include <QSettings>

#include "serverthread.h"


ServerThread::ServerThread(QObject *parent,int desc) :
    QThread(parent)
{
    TotalBytes = 0;
    fileNameSize = 0;
    bytesReceived = 0;
    mm = 0;
    isStop = true;

    fileSize = 0;
    receiveClass = 0;
    socket = new QTcpSocket();

    checkTimer = new QTimer();

    socket->setSocketDescriptor(desc);
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(ReceiveCmdFile()));
    QObject::connect(socket,SIGNAL(connected()),this,SLOT(clientConnected()));
    QObject::connect(socket,SIGNAL(disconnected()),this,SLOT(clientDisconnect()));
}
ServerThread::~ServerThread()
{
    socket->close();
    uartSocket->close();
    qDebug()<<"Call the ~founction";

}
void ServerThread::clientConnected()
{
    qDebug()<<"client connect the server";
    emit SigConnected(true);
}
void ServerThread::clientDisconnect()
{
    emit SigConnected(false);
    qDebug()<<"client disconnetc from server";
}

void ServerThread::ReceiveCmdFile()
{
    QDataStream in;
    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_4_8); //设置流版本，以防数据格式错误

    if(bytesReceived<(sizeof(qint64)+sizeof(qint8)))
    {
        //获取数据包的长度和类型
        if(socket->bytesAvailable()>=(sizeof(qint64)+sizeof(qint8)))
        {
            in>>TotalBytes;
            in>>receiveClass;
            bytesReceived += (sizeof(qint64)+sizeof(qint8));
            if(receiveClass==20)
            {
                //删除目录下已经存在的所有gcode文件
                QFile::remove(current_file_name);
                localFile = new QFile(current_file_name);
                if(!localFile->open(QFile::WriteOnly))
                {
                    return;
                }
                qDebug()<<"open successful   "<<receiveClass;
            }

        }
    }
    //如果 等于 10，则接收指令行
    if((bytesReceived>=(sizeof(qint64)+sizeof(qint8)))&&(receiveClass==10))
    {
        //直接将接受到的命令发送到下位机
        in>>command;
        bytesReceived += command.toLatin1().size();
        emit SigCmd(command);

        command = "";

    }
    //等于21 表示发送的是文件信息
    if((bytesReceived>=(sizeof(qint64)+sizeof(qint8)))&&(receiveClass==21))
    {
        //直接将接受到的命令发送到下位机
        in>>command;
        current_file_name = command;

        QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
        p->beginGroup("Config");
        p->setValue("current_file",current_file_name);
        p->endGroup();
        delete  p;

        bytesReceived += command.toLatin1().size();
        qDebug()<<"文件名为:"<<current_file_name;
        command = "";
    }
    //如果 等于 20，则接收文件
    if((bytesReceived>=(sizeof(qint64)+sizeof(qint8)))&&(receiveClass==20))
    {
        if(bytesReceived<TotalBytes)
        {
            bytesReceived += socket->bytesAvailable();
            inBlock = socket->readAll();
            mm += localFile->write(inBlock);
            qDebug()<<tr("successed write=%1").arg(mm);
            inBlock.resize(0);
        }
    }
    qDebug()<<"received:"<<bytesReceived<<",total byte:"<<TotalBytes;

    //结束接收
    if((bytesReceived==TotalBytes)&&(TotalBytes!=0))
    {
        qDebug()<<tr("successed received!");
        bytesReceived = 0;
        TotalBytes = 0;
        mm = 0;

        if(receiveClass==20)
        {
            emit SigLoaded(current_file_name);
            localFile->close();   //接收完文件后，一定要关闭，不然收到的文件会比原来的更小
            receiveClass=0;
        }
        else
        {
            command = "";
            receiveClass=0;
        }
    }

}

void ServerThread::sendCommand(QString cmd)       //发送指令
{
    qint64 totalBytes = cmd.toLatin1().size();
    //文件总大小
    QDataStream sendOut(&inBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_8);

    //这里的总大小是文件名大小等信息和实际文件大小的总和
    totalBytes += (sizeof(qint8)+sizeof(qint64));

    sendOut.device()->seek(0);

    //totalBytes是文件总大小  10 是类型
    sendOut<<totalBytes<<qint8(10)<<cmd;

    //发送完头数据后剩余数据的大小，即文件实际内容的大小
    socket->write(inBlock);


    inBlock.resize(0);
}
void ServerThread::sendCmdToArduino(QString command)
{


}





