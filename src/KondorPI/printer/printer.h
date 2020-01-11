#ifndef PRINTER_H
#define PRINTER_H

#include <QTimer>
#include <QObject>
#include <QSerialPort>

#include <QNetworkReply>
#include <QNetworkRequest>

#include "camera/qcameraer.h"
#include "printer/hardware/serialport/cpserialport.h"

using namespace cv;
using namespace std;

enum status
{
    P_IDLE=0,
    P_PRINTING=1,
    P_HUNG=2,
    P_RECOVER=3,
};

class Printer : public QObject
{
    Q_OBJECT
public:
    explicit Printer(QObject *parent = 0);
    ~Printer();
    void ConnectPort();
    void DisConnect();
    void SetStatus(status s_t);
    void LoadGcode(QString filename);
    void SendGcode(QString cmd);
    int GetPercent();

    void StartPrint();
    void PausePrint();
    void RecoverPrint();
    void CanclePrint();
    void TakePhoto();
    QStringList GetInfo();

    status current_status;

public slots:
    void WritePort(QString cmd);
    void CmdFinishedSlot(QNetworkReply *reply);


private:
    QString file_name;
    QString hand_ip;
    QNetworkAccessManager* gcode_cmd;

    void Delay(int mesc);



};

#endif // PRINTER_H
