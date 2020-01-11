#include "projector.h"
#include "ui_projector.h"
#include <QPainter>
#include <QTextStream>
#include <QDebug>
#include <QBrush>
#include <QSettings>
#include <QDir>
#include <QProcess>

QMutex Projector::mutex;
QScopedPointer<Projector> Projector::instance;

Projector::Projector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Projector)
{
    ui->setupUi(this);
    QSettings* bsetting = new QSettings("./Config.ini",QSettings::IniFormat);
    bsetting->beginGroup("Config");

    project_x = bsetting->value("g_x").toInt();
    project_y = bsetting->value("g_y").toInt();
    QString m = tr("blank.jpg");
    bsetting->endGroup();
    bank.load(m);
    m_pDesktop = new QDesktopWidget();

    frameBuffer = new DLPcontrol();
}

Projector::~Projector()
{
    delete ui;
}

//单例获取
Projector& Projector::getInstance()
{
    if(instance.isNull())
    {
        mutex.lock();
        if(instance.isNull())
        {
            instance.reset(new Projector());
        }
        mutex.unlock();
    }
    return *instance.data();
}
void Projector::showBase()
{
//    FrontEnd::getInstance().sendGcode("M805");
    QImage base("base.jpg");

//    ui->LayerImage->setPixmap(QPixmap::fromImage(base));
    writeFrameBuffer(combineFull(base));
    emit Sig_Img(base);
}
void Projector::showBlank()
{
#ifdef WIN32
    writeFrameBuffer(combineFull(bank));
#endif
    frameBuffer->drallBlank();
    emit Sig_Img(bank);
}
void Projector::showImage(QImage img)
{
//    FrontEnd::getInstance().sendGcode("M805");
//    QImage f = imgEffect->process(img);
//    writeFrameBuffer(f);
    writeFrameBuffer(combineFull(img));
    emit Sig_Img(img);
}
void Projector::writeFrameBuffer(QImage src)
{
//#ifdef _linux
    int width = src.width();
    int height = src.height();
    QRgb *pixels = (QRgb *)src.scanLine(0);
    frameBuffer->setWH((float)width,(float)height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int gray = qGray(pixels[y*width+x]);
            frameBuffer->PutPixel(x,y,gray,gray,gray);
        }
    }
//#endif

#ifdef WIN32
//     ui->LayerImage->setPixmap(QPixmap::fromImage(src));
#endif

}
QImage Projector::combineFull(QImage draw)
{
    QImage back = QImage(project_x,project_y, QImage::Format_ARGB32_Premultiplied);
    back.fill(Qt::black);
    QPainter painter;
    painter.begin((&back));
    QPoint s = QPoint((project_x - draw.width()) / 2,(project_y - draw.height()) / 2);
    painter.drawImage(s, draw);
    painter.end();

    return back;
}
void Projector::connectProject()
{

    #ifdef WIN32
    int i= 0;
        int screenCount = m_pDesktop->screenCount();
        QString a = tr("screencount = %1").arg(screenCount);

        QRect screenGeometry;
        if(screenCount==1)
        {
            screenGeometry = m_pDesktop->screenGeometry(0);
            QMessageBox::information(this,tr("提示"),tr("没有检测到光源！"));
        }
        else if(screenCount>=2)
        {
            bool projectb = false;
            for(i=screenCount-1;i>= 1;i--)
            {
                screenGeometry = m_pDesktop->screenGeometry(i);

                if(screenGeometry.width() == project_x && screenGeometry.height() == project_y)
                {
                    //Found the projector!
                    QString sVideo = "Connected to Monitor: " + QString::number(i+1);
                    qDebug()<<sVideo;
                    projectb = true;
                    //                QString strStyle = "QPushButton {color: rgb(25, 136, 214);}";
                    //                ui->ProjectConect->setStyleSheet(strStyle);
                    this->setGeometry(screenGeometry);
                    qDebug()<<tr("链接投影仪完毕");

                    this->showFullScreen();
                    activateWindow();
                    break;
                }
            }

        }
        showBlank();
#endif

//#ifdef _linux
        QImage f("./FocusHD.jpg");
        writeFrameBuffer(f);
//#endif

}
