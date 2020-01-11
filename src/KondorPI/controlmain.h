#ifndef CONTROLMAIN_H
#define CONTROLMAIN_H

#include <QTimer>
#include <QMainWindow>
#include <QNetworkInterface>

#include "printer/printer.h"
#include "server/mserver.h"

#include "interface/printset.h"
#include "interface/controltool.h"
#include "interface/configset.h"

namespace Ui {
class ControlMain;
}

class ControlMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlMain(QWidget *parent = nullptr);
    ~ControlMain();

public slots:

private:
    Ui::ControlMain *ui;
    bool b_online;
    bool b_printing;
    bool b_paused;


    Printer* m_printer;
    MServer* server;
    QThread* serverThread;

    ConfigSet* system_set;
    PrintSet* printer_set;
    ControlTool* tool_set;

    void Delay(int mesc);

private slots:
    void SlotCmd(QString cmd);
    void SlotNetConnect(bool b_connect);
    void on_btnPrint_clicked();
    void on_btnSystem_clicked();
    void on_btnTools_clicked();
    void on_btnReboot_clicked();
};

#endif // CONTROLMAIN_H
