#include <QDebug>
#include <QRegExp>
#include <QTimer>
#include <QHostInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSerialPortInfo>

#include "controlmain.h"
#include "ui_controlmain.h"

#include "tools/easylogging++.h"

ControlMain::ControlMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlMain)
{
    ui->setupUi(this);
    setWindowFlags (Qt::FramelessWindowHint);
    m_printer = new Printer();

    server = new MServer();
    serverThread = new QThread();
    server->moveToThread(serverThread);
    serverThread->start();
    connect(server,SIGNAL(SigConnect(bool)),this,SLOT(SlotNetConnect(bool)));
    connect(server,SIGNAL(SigUpload(QString)),this,SLOT(SlotNetSendFile(QString)));
    connect(server,SIGNAL(SigCmd(QString)),this,SLOT(SlotCmd(QString)));

    system_set = new ConfigSet();
    printer_set = new PrintSet();
    tool_set = new ControlTool();
}

ControlMain::~ControlMain()
{
    delete m_printer;
    delete ui;
}

void ControlMain::Delay(int mesc)
{
    QEventLoop eventloop;
    QTimer::singleShot(mesc,&eventloop,SLOT(quit()));
    eventloop.exec();
}


void ControlMain::SlotCmd(QString cmd)
{
    if(cmd=="PausePrint")
    {
        printer_set->on_btnRecovery_clicked();
    }
    else if(cmd=="StartPrint")
    {
        printer_set->on_btnStart_clicked();

    }
    else if(cmd.startsWith("CanclePrint"))
    {
        printer_set->on_btnCancle_clicked();
    }
    else if(cmd.startsWith("RemoveCache"))
    {
        QDir* dir = new QDir(".");
        QStringList filter;
        filter<<"*.bgz"<<"*.slc";
        dir->setNameFilters(filter);
        QList<QFileInfo>* file_infos = new QList<QFileInfo>(dir->entryInfoList(filter));
        for(int ts=0;ts<file_infos->count();ts++)
        {
            QFile::remove(file_infos->at(ts).filePath());
        }
    }
    else
    {
        m_printer->SendGcode(cmd);
    }
}

void ControlMain::SlotNetConnect(bool b_connect)
{
    if(b_connect)
    {
        LOG(DEBUG)<<"client connected!";

    }
    else
    {
        LOG(DEBUG)<<"client disconnected!";
    }
}

//打开选择打印按钮
void ControlMain::on_btnPrint_clicked()
{
#ifdef Q_OS_LINUX
    printer_set->showFullScreen();
#elif defined (Q_OS_WIN32)
    printer_set->showNormal();
#endif

}

//打开系统按钮
void ControlMain::on_btnSystem_clicked()
{
#ifdef Q_OS_LINUX
    system_set->showFullScreen();
#elif defined (Q_OS_WIN32)
    system_set->showNormal();
#endif
}

//打开工具按钮
void ControlMain::on_btnTools_clicked()
{
#ifdef Q_OS_LINUX
    tool_set->showFullScreen();
#elif defined (Q_OS_WIN32)
    tool_set->showNormal();
#endif
}
//重启按钮
void ControlMain::on_btnReboot_clicked()
{
    QProcess process;
    QStringList options;
    options<<"-c";
    options<<"sudo reboot";
    process.start("/bin/bash", options);
    process.waitForFinished(-1);
}
