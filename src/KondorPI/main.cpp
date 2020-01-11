#include "controlmain.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>

#include "tools/easylogging++.h"
#include "tools/keyboard/frmnum.h"
#include "tools/keyboard/frminput.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f(":qdarkstyle/style.qss");
    el::Configurations conf("logconfig.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    if (!f.exists())
    {
        LOG(ERROR) << "Unable to set stylesheet, file not found\n";
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
#ifdef Q_OS_LINUX
    frmInput::Instance()->Init("bottom", "black", 10, 10);
#elif defined (Q_OS_WIN32)
    frmInput::Instance()->Init("control", "black", 10, 10);
#endif

    ControlMain w;
    w.showFullScreen();
#ifdef Q_OS_LINUX
    w.showFullScreen();
#elif defined (Q_OS_WIN32)
    w.showNormal();
#endif

    return a.exec();
}
