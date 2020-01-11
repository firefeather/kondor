#ifndef CONTROLTOOL_H
#define CONTROLTOOL_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "printer/hardware/projector/projector.h"
#include "printer/hardware/serialport/cpserialport.h"

namespace Ui {
class ControlTool;
}

class ControlTool : public QWidget
{
    Q_OBJECT

public:
    explicit ControlTool(QWidget *parent = 0);
    ~ControlTool();
    void SendCmd(QString cmd);

public slots:
    void CmdFinishedSlot(QNetworkReply *reply);
    void SlotAutoConnect();
private slots:
    void on_btnBack_clicked();

    void on_radioButton1_clicked();

    void on_radioButton10_clicked();

    void on_radioButton100_clicked();

    void on_z_add_clicked();

    void on_z_sub_clicked();

    void on_Home_clicked();

    void on_btnBottom_clicked();

    void on_btnZMax_clicked();

    void on_btnLCDOn_clicked();

    void on_btnGird_clicked();

    void on_btnReset_clicked();

    void on_btnFanOn_clicked();

    void on_btnReboot_clicked();

private:
    Ui::ControlTool *ui;
    float step_unit;
    double z_height;
    bool b_lcd_light;
    bool b_fan_on;

    QString hand_ip;
    QNetworkAccessManager* gcode_cmd;

};

#endif // CONTROLTOOL_H
