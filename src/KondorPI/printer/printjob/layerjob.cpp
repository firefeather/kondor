#include "layerjob.h"

LayerJob::LayerJob(QObject *parent) :
    QObject(parent)
{
    QSettings settings("Config.ini",QSettings::IniFormat);
    settings.beginGroup("formPrinter");

    pFirstExposeTime = settings.value("pFirstExposeTime").toDouble();
    pSupportExposeTime = settings.value("pSupportExposeTime").toDouble();
    pModelExposeTime = settings.value("pModelExposeTime").toDouble();
    zTotalDistance = settings.value("z_total").toDouble();

    layerThickness = settings.value("layerThickness").toDouble();
    slowUpDistance = settings.value("slowUpDistance").toDouble();
    upDownDistance = settings.value("upDownDistance").toDouble();

    currentLay = settings.value("currentLay").toInt();
    baseLayNum = settings.value("baseLayNum").toInt();
    dividLayNum = settings.value("dividLayNum").toInt();
    slowUpSpeed = settings.value("slowUpSpeed").toInt();
    normalSpeed = settings.value("normalSpeed").toInt();
    zTotalTime = settings.value("zTotalTime").toInt();
    lightHide = settings.value("lightHide").toDouble();
    settings.endGroup();

    bgzSupportHeight = 0;
    bgzSupportMaxHeight = 0;

    connect(this,SIGNAL(Sig_ToFrontEnd(QString)),&FrontEnd::getInstance(),SLOT(sendGcode(QString)));
}
void LayerJob::showBlank()
{
    Projector::getInstance().showBlank();
}
void LayerJob::closeLight()
{
//    sendToArduino("G90");
//    sendToArduino("G1 Y0");
//    sendToArduino("G91");
}
void LayerJob::openLight()
{
//    sendToArduino("G90");
//    sendToArduino(tr("G1 Y%1").arg(lightHide));
//    sendToArduino("G91");
}

void LayerJob::beforeExpose()
{
    //先上升到顶部，再降落到底部
    sendToArduino(tr("G28 X F%1").arg(normalSpeed));
    sendToArduino(tr("G1 X%1 F%2").arg(zTotalDistance).arg(normalSpeed));
//    cDelay(zTotalTime*1000);
    cDelay(1000);

    //Open the projector
    sendToArduino("M800");
    sendToArduino("G91");
    cDelay(8000);
//    cDelay(8000);
}
//开始曝光
void LayerJob::exposeLayer(int layNum)
{
    currentLay = layNum;

    if(layNum<2)
    {
        int layerTime = pModelExposeTime+pSupportExposeTime+calcuteMovingTime(normalSpeed,upDownDistance);
        int t = totalLayNum*layerTime+200;
        int hour = t/3600;
        int min = t%3600/60;
        Log(tr("开始曝光..."));
        Log(tr("预计耗时:%1小时%2分钟").arg(hour).arg(min));
    }

    slotExposeA();

}

//读取一层，并开始曝光
void LayerJob::slotExposeA()
{
    qDebug()<<"slotExposeA";


//    currentImg.save(tr("%1.jpg").arg(currentLay),"JPEG",100);
    //底座曝光时间
    openLight();
    cDelay(100);
    if(currentLay<=baseLayNum)
    {
        Projector::getInstance().showBase();
        QTimer::singleShot((pFirstExposeTime*1000),this,SLOT(slotAfterExpose()));
//        Log(tr("底座曝光时间:%1").arg(pFirstExposeTime));
    }
    //分离层曝光时间
    else if((currentLay>baseLayNum)&&(currentLay<dividLayNum))
    {
        Projector::getInstance().showImage(currentImg);
        QTimer::singleShot((pSupportExposeTime*1000),this,SLOT(slotAfterExpose()));
    }
    //正常曝光时间
    else
    {
        Projector::getInstance().showImage(currentImg);
//        if(currentLay<15)
//            currentImg.save(tr("%1_t.jpg").arg(currentLay),"JPEG",100);
        QTimer::singleShot((pModelExposeTime*1000),this,SLOT(slotAfterExpose()));
    }

}


//曝光bgz文件
void LayerJob::exposeBgzLayer(int layNum)
{
    currentLay = layNum;

    openLight();
    cDelay(100);
    if(layNum<2)
    {
        int layerTime = pModelExposeTime+pSupportExposeTime+calcuteMovingTime(normalSpeed,upDownDistance);
        int t = totalLayNum*layerTime+200;
        int hour = t/3600;
        int min = t%3600/60;
        Log(tr("预计耗时:%1小时%2分钟").arg(hour).arg(min));
    }
    slotExposeBgz();
}

//模型曝光完成,只在支撑模型分离的情况下使用
void LayerJob::slotExposeBgz()
{
    if(currentLay<=baseLayNum)
    {
        //曝光模型底坐
        Projector::getInstance().showBase();
        QTimer::singleShot((pFirstExposeTime*1000),this,SLOT(slotAfterExpose()));
    }
    else if((currentLay>baseLayNum)&&currentLay<bgzSupportHeight)
    {
        //只曝光支撑
        Projector::getInstance().showImage(currentSupportImg);
        QTimer::singleShot(((pSupportExposeTime+pModelExposeTime)*1000),this,SLOT(slotAfterExpose()));

    }
    else
    {
        if(currentLay<bgzSupportMaxHeight)
        {
            //先曝光支撑，再跳转曝光模型
            Projector::getInstance().showImage(currentSupportImg);
            QTimer::singleShot((pSupportExposeTime*1000),this,SLOT(slotExposeBgzSupport()));
        }
        else
        {
            //只曝光模型
            Projector::getInstance().showImage(currentImg);
            QTimer::singleShot((pModelExposeTime*1000),this,SLOT(slotAfterExpose()));
        }
    }
}

void LayerJob::slotExposeBgzSupport()
{
    Projector::getInstance().showImage(currentImg);
    QTimer::singleShot((pModelExposeTime*1000),this,SLOT(slotAfterExpose()));
}

void LayerJob::slotAfterExpose()
{
    closeLight();
    qDebug()<<tr("slotAfterExpose");
    //曝光完成后，首先将投影切换成黑色背景
    Projector::getInstance().showBlank();
    //如果是LED光源，可在次关掉灯泡开关

    Log(tr("非曝光时间开始"));
    //开始慢速向上
    cDelay(300);
    slotSlowUp();

//    slotUpDownAll();
}

void LayerJob::slotUpDownAll()
{
    QString cmd1 = tr("G1 X-%1 F%2").arg(slowUpDistance).arg(slowUpSpeed);
    sendToArduino(cmd1);

    QString cmd2 = tr("G1 X-%1 F%2").arg(upDownDistance-slowUpDistance).arg(normalSpeed);
    sendToArduino(cmd2);

    double dist = upDownDistance - layerThickness;
    QString cmd = tr("G1 X%1 F%2").arg(dist).arg(normalSpeed);
    sendToArduino(cmd);

    double time = 1+calcuteMovingTime(slowUpSpeed,(slowUpDistance))+calcuteMovingTime(normalSpeed,(upDownDistance-slowUpDistance))+calcuteMovingTime(normalSpeed,(upDownDistance - layerThickness));
    cDelay(time*1000);
    qDebug()<<tr("开始下一层");
    emit Sig_NextLayer();

}

//曝光完成，慢速拉升
void LayerJob::slotSlowUp()
{
    QString cmd = tr("G1 X-%1 F%2").arg(slowUpDistance).arg(slowUpSpeed);
    sendToArduino("G91");
    sendToArduino(cmd);

    //预留1s上升时间
    double time = calcuteMovingTime(slowUpSpeed,(slowUpDistance));
    Log(tr("慢速拉升时间=%1秒").arg(time));
    cDelay(time*1000);
    slotNormalUp();
}

 //正常速度上升
void LayerJob::slotNormalUp()
{
    QString cmd = tr("G1 X-%1 F%2").arg(upDownDistance-slowUpDistance).arg(normalSpeed);
    sendToArduino("G91");
    sendToArduino(cmd);
    double time = calcuteMovingTime(normalSpeed,(upDownDistance-slowUpDistance));
    Log(tr("常速拉升时间=%1秒").arg(time));
    cDelay(time*1000);
    slotNormalDown();
}



//正常速度下降
void LayerJob::slotNormalDown()
{
    qDebug()<<tr("slotNormalDown");
    double dist = upDownDistance - layerThickness;
    QString cmd = tr("G1 X%1 F%2").arg(dist).arg(normalSpeed);
//    sendToArduino("G91");
    sendToArduino(cmd);
    double time = 1+calcuteMovingTime(normalSpeed,(upDownDistance - layerThickness));
    cDelay(time*1000);
    Log(tr("正在打印第%1层，剩余%2层").arg(currentLay).arg(totalLayNum-currentLay));
    qDebug()<<tr("常速下降时间=%1秒").arg(time);

    qDebug()<<tr("开始下一层");
    emit Sig_NextLayer();
}

void LayerJob::Log(QString s)
{
    CPLog::getInstance().setLogContent(s);
}

void LayerJob::recvFromArduino(QString le)
{
    //慢速上升完成,正常速度上升

//    if(le.contains(QRegExp(".*&.*")))
//    {
//        qDebug()<<"&&&&&&&&";
//        slotNormalUp();
//    }
//    //正常上升完成,正常速度下降
//    else if(le.contains(QRegExp(".*\\*.*")))
//    {
//        qDebug()<<"*****";
//        slotNormalDown();
//    }
//    //正常下降完成
//    else if(le.contains(QRegExp(".*@.*")))
//    {
//        qDebug()<<"@@@@@";
//        emit Sig_NextLayer();
//    }

}
double LayerJob::calcuteMovingTime(int speed,double distance)
{
    double speeds = speed/60.0;  //  mm/min---->mm/s
    double time = distance/speeds;
    return time;
}

void LayerJob::cDelay(int mesc)
{
    QEventLoop eventloop;
    QTimer::singleShot(mesc,&eventloop,SLOT(quit()));
    eventloop.exec();
//    usleep(1000*mesc);
}
void LayerJob::sendToArduino(QString cmd)
{
//    FrontEnd::getInstance().sendGcode(cmd);
    emit Sig_ToFrontEnd(cmd);
}
