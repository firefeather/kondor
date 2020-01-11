#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QSocketNotifier>
#include <QDataStream>
#include <QMutex>
#include <QTimer>


class ServerThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerThread(QObject *parent = 0,int desc=0);
    ~ServerThread();

signals:
    void out();
public slots:
    void ReceiveCmdFile();
    void sendCmdToArduino(QString command);  
    void clientDisconnect();
    void clientConnected();
private:
    QTcpSocket* socket;
    QString tosend;
    QString current_file_name;
    bool isStop;
    QMutex mutex;
    QTimer* checkTimer;

    qint64 TotalBytes;
    qint64 bytesReceived;
    qint64 bytesToReceive;
    qint64 fileNameSize;

    qint64 mm;

    qint64 fileSize;
    qint8 receiveClass;

    QString fileName;
    QString command;
    QFile *localFile;
    QByteArray inBlock;

    QDataStream in;

    QSocketNotifier *uartListen;
    QTcpSocket* uartSocket;

    void addToSender();
    void sendCommand(QString);

signals:
    void SigCmd(QString);
    void SigLoaded(QString);
    void SigConnected(bool);

};

#endif // SERVERTHREAD_H
