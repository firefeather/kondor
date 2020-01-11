#include <QDir>
#include "printset.h"
#include "ui_printset.h"

PrintSet::PrintSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintSet)
{
    ui->setupUi(this);
    setWindowFlags (Qt::FramelessWindowHint);

    st_type = new StorageType();
    print_data = new PrintFileData();
    lcd_printer = new Printer();
    usb_type = new USBStorageFile();
    printer_status = PRINT_IDLE;
    current_layer = 0;
    up_down_action = 0.0;


    connect(&Projector::getInstance(),SIGNAL(Sig_Img(QImage)),this,SLOT(SlotPrintPreview(QImage)));
    connect(st_type,SIGNAL(SigPreview()),this,SLOT(SlotGenPreview()));
    connect(usb_type,SIGNAL(SigPreview()),this,SLOT(SlotGenPreview()));
}

PrintSet::~PrintSet()
{
    delete ui;
}

void PrintSet::Delay(int mesc)
{
    QEventLoop eventloop;
    QTimer::singleShot(mesc,&eventloop,SLOT(quit()));
    eventloop.exec();
}

bool PrintSet::removeDirWithContent(QString dirName)
{
    static QVector<QString> dirNames;

    QDir dir;
    QFileInfoList filst;
    QFileInfoList::iterator curFi;

    //初始化
    dirNames.clear();
    if(dir.exists()){
        dirNames<<dirName;
    }
    else{
        return true;
    }

    //遍历各级文件夹，并将这些文件夹中的文件删除
    for(int i=0;i<dirNames.size();++i)
    {
        dir.setPath(dirNames[i]);
        filst=dir.entryInfoList(QDir::Dirs|QDir::Files
                                |QDir::Readable|QDir::Writable
                                |QDir::Hidden|QDir::NoDotAndDotDot
                                ,QDir::Name);
        if(filst.size()>0)
        {
            curFi=filst.begin();
            while(curFi!=filst.end())
            {
                //遇到文件夹,则添加至文件夹列表dirs尾部
                if(curFi->isDir())
                {
                    dirNames.push_back(curFi->filePath());
                }
                else if(curFi->isFile())
                {
                    //遇到文件,则删除之
                    if(!dir.remove(curFi->fileName()))
                    {
                        return false;
                    }
                }
                curFi++;
            }//end of while
        }
    }

    //删除文件夹
    for(int i=dirNames.size()-1;i>=0;--i)
    {
        dir.setPath(dirNames[i]);
        if(!dir.rmdir(".")){
            return false;
        }
    }

    return true;
}

void PrintSet::SlotGenPreview()
{
    LoadPrintData();
    QImage current_img = print_data->getLayerImage(5);
    SlotPrintPreview(current_img);
}

void PrintSet::SlotPrintPreview(QImage s)
{
    QMatrix matrix;
    matrix.rotate(90.0);//以90度为例
    QImage image = s.transformed(matrix,Qt::FastTransformation);
    ui->labelPreview->setPixmap(QPixmap::fromImage(image));


    int w = ui->labelPreview->width();
    int h = ui->labelPreview->height();
    //    QImage currentL = pProjector->getCurrentImage();
    double scalew = w*1.0/s.width();
    double scaleh = h*1.0/s.height();
    double scale = qMin(scalew,scaleh);
    QImage s1 = image.scaled(image.width()*scale,image.height()*scale,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->labelPreview->setPixmap(QPixmap::fromImage(s1));
}
void PrintSet::CmdFinishedSlot(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug()<<"get reply error!";
    }
    else
    {
        QByteArray responseData = reply->readAll();
        qDebug()<<"Tmp:"<<responseData;
    }
}

void PrintSet::LoadPrintData()
{
    QDir *temp = new QDir();
    bool exist = temp->exists("./project");
    if(exist)
        removeDirWithContent(("./project"));
    bool ok = temp->mkdir("./project");

    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    QString name = p->value("current_file").toString();
    double z_height = p->value("z_height").toDouble();
    p->endGroup();
    delete  p;

    print_data->loadPrintFile(name);
    qDebug()<<"The total print layers:"<<print_data->getTotalLayers();
}

//开始按键
void PrintSet::on_btnStart_clicked()
{
    if(printer_status==PRINT_IDLE)
    {
        current_layer = 0;
        //1.从ini中装载当前要打印的文件
        up_down_action = 0;
        //2.移动到顶部并设置行程
        lcd_printer->SendGcode("$H");
        QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
        p->beginGroup("Config");
        double z_height = p->value("z_height").toDouble();
        p->endGroup();
        delete  p;
        lcd_printer->SendGcode(tr("G92 X0 Y0 Z%1").arg(z_height));


        //3.移动到底部
        lcd_printer->SendGcode(tr("G1 Z0 F300"));

        //4.开启定时循环
        printer_status=PRINT_PRINTING;

        //5.开启相对移动模式
        Delay(60*1000);  //give enouth time z axis home and go bottom

        SlotUpDown();

    }

}

void PrintSet::SlotUpDown()
{
    if(printer_status==PRINT_PRINTING)
    {
        lcd_printer->SendGcode("M5");
        Projector::getInstance().showBlank();
        QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
        p->beginGroup("Config");
//        double thickness = p->value("normal_layer_thickness").toDouble();
        double updown_dis = p->value("updown_dis").toDouble();
        int updown_speed = p->value("updown_speed").toInt();
        int updown_time = p->value("updown_time").toInt();
        p->endGroup();
        double thickness = print_data->getSlcThickness();
        delete  p;
        up_down_action += thickness;
        //上升
        lcd_printer->SendGcode(tr("G1 Z%1 F%2").arg(updown_dis+up_down_action).arg(updown_speed));
        Delay(500);
        //下降
        lcd_printer->SendGcode(tr("G1 Z%1 F%2").arg(up_down_action).arg(updown_speed));

        qDebug()<<"Now in the slot up and down";
        QTimer::singleShot((updown_time*1000),this,SLOT(SlotExposeImg()));
    }
}

void PrintSet::SlotExposeImg()
{
    QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
    p->beginGroup("Config");
    int expose_time = p->value("normal_expose_time").toInt();
    int init_expose_time = p->value("init_expose_time").toInt();
    int init_layers = p->value("init_layer_count").toInt();
    p->endGroup();
    delete  p;

    QImage current_img = print_data->getLayerImage(current_layer);
    //    current_img.save(tr("%1.jpg").arg(current_layer),"JPEG",100);
    lcd_printer->SendGcode("M3");
    Projector::getInstance().showImage(current_img);
    current_layer++;

    int progress = 100*(1.0*current_layer)/(print_data->getTotalLayers()*1.0);

    ui->labelProgress->setText(tr("Progress:%%1").arg(progress));
    ui->labelCurrentLayer->setText(tr("Now:%1/%2").arg(current_layer).arg(print_data->getTotalLayers()));

    if(current_layer<=init_layers)
    {
        QTimer::singleShot((init_expose_time*1000),this,SLOT(SlotUpDown()));
    }
    else if((current_layer>init_layers)&&(current_layer<print_data->getTotalLayers()))
    {
        QTimer::singleShot((expose_time*1000),this,SLOT(SlotUpDown()));
    }
    else if(current_layer>=print_data->getTotalLayers())
    {
        //打印结束

        //1.关闭LCD
        lcd_printer->SendGcode("M5");

        //2.显示黑屏
        Projector::getInstance().showBlank();

        //3.抬升平台
        lcd_printer->SendGcode("$H");

        //4.切换状态
        printer_status = PRINT_IDLE;
        current_layer = 0;
        lcd_printer->SendGcode("G90");
        qDebug()<<"Print finish!";
        ui->labelProgress->setText(tr("Progress:%%1").arg(0));
        ui->labelCurrentLayer->setText(tr("Now:%1/%2").arg(0).arg(0));

    }
}

//暂停/恢复打印按键
void PrintSet::on_btnRecovery_clicked()
{
    if(printer_status==PRINT_PRINTING)
    {
        QSettings* p = new QSettings("Config.ini",QSettings::IniFormat);
        p->beginGroup("Config");
        int expose_time = p->value("normal_expose_time").toInt();
        p->endGroup();
        delete  p;

        ui->btnRecovery->setText(tr("Recover"));
        printer_status = PRINT_PAUSE;
        //延时曝光完这一层之后，再关闭LCD的曝光
        Delay(expose_time*1000);
        //关闭LCD
        Projector::getInstance().showBlank();
        lcd_printer->SendGcode("M5");

        //暂停之后，上升10mm
        lcd_printer->SendGcode("G91");
        lcd_printer->SendGcode(tr("G1 Z%1 F300").arg(20));
        lcd_printer->SendGcode("G90");
    }
    else if(printer_status==PRINT_PAUSE)
    {
        //恢复打印，先下降一定的高度
        lcd_printer->SendGcode("G91");
        lcd_printer->SendGcode(tr("G1 Z%1 F300").arg(-20));
        lcd_printer->SendGcode("G90");
        Delay(10000);
        printer_status=PRINT_PRINTING;
        SlotUpDown();
        ui->btnRecovery->setText(tr("Pause"));
    }
}
//结束按键
void PrintSet::on_btnCancle_clicked()
{
    lcd_printer->SendGcode("G90");
    printer_status=PRINT_IDLE;
    //1.关闭LCD
    lcd_printer->SendGcode("G4 P4");
    lcd_printer->SendGcode("M5");

    //2.显示黑屏
    Projector::getInstance().showBlank();

    //3.抬升平台
    lcd_printer->SendGcode("$H");

    //4.切换状态
    printer_status = PRINT_IDLE;
    current_layer = 0;
    lcd_printer->SendGcode("G90"); //绝对模式
    ui->btnRecovery->setText(tr("Pause"));
}

void PrintSet::SetPrintPersent(int persent)
{

}


void PrintSet::on_btnBack_clicked()
{
    this->close();
}
//转载文件
void PrintSet::on_btnLoadFile_clicked()
{
#ifdef Q_OS_LINUX
    st_type->showFullScreen();
#elif defined (Q_OS_WIN32)
    st_type->showNormal();
#endif
    st_type->UpdateTree();
}

//USB文件获取
void PrintSet::on_btnUSBFile_clicked()
{
#ifdef Q_OS_LINUX
    usb_type->showFullScreen();
    usb_type->UpdateTree();
#elif defined (Q_OS_WIN32)
    usb_type->showNormal();
#endif

}
