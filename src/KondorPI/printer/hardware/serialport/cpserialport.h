#ifndef CPSERIALPORT_H
#define CPSERIALPORT_H

#include <QWidget>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class CPSerialPort;
}

class CPSerialPort : public QThread
{
    Q_OBJECT

public:
    static CPSerialPort& getInstance();

    void UpdateLanguage();
    void ConnectPort(QString comName);
    void Disconnect();

public slots:
    void Slot_ResponseUart();
    void WritePort(QString str);


private:
    CPSerialPort(QObject *parent = 0);
    CPSerialPort(const CPSerialPort &other);
    CPSerialPort& operator=(const CPSerialPort &other);

    static QMutex mutex;
    static QScopedPointer<CPSerialPort>instance;
    friend struct QScopedPointerDeleter<CPSerialPort>;


    QSerialPort *mySerialport;
    QStringList comList;
    bool bConnect;
signals:
    void Sig_GetMessage(QString);
    void Sig_disconnect();

};

#endif // CPSERIALPORT_H
