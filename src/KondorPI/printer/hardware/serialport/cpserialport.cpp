#include "cpserialport.h"


QMutex CPSerialPort::mutex;
QScopedPointer<CPSerialPort> CPSerialPort::instance;


CPSerialPort::CPSerialPort(QObject *parent) :
    QThread(parent)
{

    mySerialport = new QSerialPort();
    bConnect = false;
    //init serial and show it in the UI combo

}


CPSerialPort& CPSerialPort::getInstance()
{
    if(instance.isNull())
    {
        mutex.lock();
        if(instance.isNull())
        {
            instance.reset(new CPSerialPort());
        }
        mutex.unlock();
    }
    return *instance.data();
}

void CPSerialPort::UpdateLanguage()
{

}

void CPSerialPort::Slot_ResponseUart()
{
    QString po = mySerialport->readAll();
//    QString po = mySerialport->readLine();
    emit Sig_GetMessage(po);
}
void CPSerialPort::WritePort(QString str)
{
    if(bConnect)
    {
        QString s = str + "\n";
        mySerialport->write(s.toLatin1());
    }

}



void CPSerialPort::ConnectPort(QString comName)
{
    if(!bConnect)
    {
        mySerialport->setPortName(comName);
        mySerialport->setBaudRate(115200);
        mySerialport->setDataBits(QSerialPort::Data8);
        mySerialport->setParity(QSerialPort::NoParity);
        mySerialport->setStopBits(QSerialPort::OneStop);
        mySerialport->setFlowControl(QSerialPort::NoFlowControl);
        if(mySerialport->open(QIODevice::ReadWrite))
        {
            connect(mySerialport,SIGNAL(readyRead()),this,SLOT(Slot_ResponseUart()));

            bConnect = true;
            //QMessageBox::information(this,tr("Notice"),tr("连接成功"));
        }


    }
    else
    {
        bConnect = false;

        emit Sig_disconnect();
        mySerialport->disconnect();
        mySerialport->close();
    }
}

void CPSerialPort::Disconnect()
{
    mySerialport->disconnect();
    mySerialport->close();
}

