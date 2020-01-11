#ifndef WIFISSID_H
#define WIFISSID_H

#include <QWidget>

#include "wifipwd.h"

namespace Ui {
class WifiSSID;
}

class WifiSSID : public QWidget
{
    Q_OBJECT

public:
    explicit WifiSSID(QWidget *parent = nullptr);
    ~WifiSSID();

private slots:
    void on_btnBack_clicked();

    void on_btnWifiOK_clicked();

    void on_btnScan_clicked();

private:
    Ui::WifiSSID *ui;
    WifiPWD* pwd;
};

#endif // WIFISSID_H
