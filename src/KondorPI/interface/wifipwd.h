#ifndef WIFIPWD_H
#define WIFIPWD_H

#include <QWidget>

namespace Ui {
class WifiPWD;
}

class WifiPWD : public QWidget
{
    Q_OBJECT

public:
    explicit WifiPWD(QWidget *parent = nullptr);
    ~WifiPWD();

private slots:
    void on_btnBack_clicked();

    void on_btnWifiOK_clicked();

private:
    Ui::WifiPWD *ui;
};

#endif // WIFIPWD_H
