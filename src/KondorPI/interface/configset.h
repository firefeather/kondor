#ifndef CONFIGSET_H
#define CONFIGSET_H

#include <QWidget>
#include <QHostAddress>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkInterface>

#include "wifissid.h"

namespace Ui {
class ConfigSet;
}

class ConfigSet : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigSet(QWidget *parent = nullptr);
    ~ConfigSet();
public slots:
    void CmdFinishedSlot(QNetworkReply *reply);


private slots:
    void on_btnBack_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBoxCounts_valueChanged(int arg1);

    void on_spinBoxNormalTime_valueChanged(int arg1);

    void on_doubleSpinBoxThickness_valueChanged(double arg1);

    void on_btnSetWifi_clicked();

private:
    Ui::ConfigSet *ui;
    QNetworkAccessManager* gcode_cmd;
    WifiSSID* ssid;
};

#endif // CONFIGSET_H
