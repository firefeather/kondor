
#include <QDebug>
#include "printjob.h"

PrintJob::PrintJob(QObject *parent) :
    QObject(parent)
{
    bStop = true;
    bBgz = false;
    currentLayerCount = 1;
    totalLayer = 0;
    startLayer = 0;

    bgzSupportHeight = 0;
    bgzSupportMaxHeight = 0;

    QSettings settings("Config.ini",QSettings::IniFormat);
    settings.beginGroup("formPrinter");
    //投影仪的分辨率
    startLayer = settings.value("startlayer").toInt();
    settings.endGroup();

    layer = new LayerJob();
    printdata = new PrintFileData();
    connect(layer,SIGNAL(Sig_NextLayer()),this,SLOT(slotNextLayer()));
}

void PrintJob::Log(QString s)
{
    CPLog::getInstance().setLogContent(s);
}
void PrintJob::sendToArduino(QString s)
{
    FrontEnd::getInstance().sendGcode(s);
}

//装载文件
void PrintJob::loadFile(QString file)
{

    bBgz = true;
    printdata->loadPrintFile(file);

    totalLayer = printdata->getTotalLayers();
    layer->setTotalLayerNum(totalLayer);
    qDebug()<<tr("总层数=%1").arg(totalLayer);
    Log(tr("总层数=%1").arg(totalLayer));
}

//开始打印，开始一个循环，获取一层，LayerJob开始工作
void PrintJob::startCycle()
{

    if(bBgz)
    {
        QImage model = printdata->getLayerImage(currentLayerCount);
        layer->setCurrentImg(model);
        layer->beforeExpose();//执行归位等动作
//        layer->exposeBgzLayer(currentLayerCount);
        layer->exposeLayer(currentLayerCount);
    }
    else
    {
        QImage s = printdata->getLayerImage(currentLayerCount);
        layer->setCurrentImg(s);
        layer->beforeExpose();
        layer->exposeLayer(currentLayerCount);
    }
    Log("开始打印任务...");

    Log(tr("当前层:%1").arg(currentLayerCount));
    bStop = false;
    currentLayerCount++;

}

//打印下一层的槽函数
void PrintJob::slotNextLayer()
{
    if(!bStop)
    {
        if(currentLayerCount<(totalLayer-startLayer))
        {
            if(bBgz)
            {
                QImage model = printdata->getLayerImage(currentLayerCount);
                layer->setCurrentImg(model);
//                layer->exposeBgzLayer(currentLayerCount);
                layer->exposeLayer(currentLayerCount);
                currentLayerCount++;
            }
            else
            {
                QImage s = printdata->getLayerImage(currentLayerCount+startLayer);
                layer->setCurrentImg(s);
                layer->exposeLayer(currentLayerCount);
                currentLayerCount++;
            }

        }
        else
        {
            //打印完成了，做重置处理
            //Close the projector
            sendToArduino("M801");
            reset();
            sendToArduino("G28 X F500");
            emit Sig_PrintFinish();
            Log(tr("打印完成！光源已经关闭！"));
        }
    }
}

//暂停打印
void PrintJob::pauseJob()
{
    bStop = true;
    layer->closeLight();
}

//恢复打印
void PrintJob::recovertJob()
{

    if(bBgz)
    {
        QImage model = printdata->getLayerImage(currentLayerCount);
        layer->setCurrentImg(model);
//        layer->exposeBgzLayer(currentLayerCount);
        layer->exposeLayer(currentLayerCount);
    }
    else
    {

        QImage s = printdata->getLayerImage(currentLayerCount);
        layer->setCurrentImg(s);
        layer->exposeLayer(currentLayerCount);
    }
    Log("恢复打印任务...");

    bStop = false;
    currentLayerCount++;
}

//取消打印
void PrintJob::cancleJob()
{
    bStop = true;
    reset();
    sendToArduino("G28 X F500");
    sendToArduino("M801");
}

//连接投影
void PrintJob::connectProjector()
{

}

void PrintJob::reset()
{
    bStop = true;
    bBgz=false;
    currentLayerCount = 0;
    totalLayer = 0;
    startLayer=0;
    bgzSupportHeight=0;
}
