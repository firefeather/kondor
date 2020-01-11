#ifndef LAYERJOB_H
#define LAYERJOB_H

#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QRegExp>

#include "../Hardware/Projector/projector.h"
#include "../Hardware/SerialPort/frontend.h"
#include "../Hardware/cplog.h"

class LayerJob : public QObject
{
    Q_OBJECT
public:
    explicit LayerJob(QObject *parent = 0);
    void showBlank();
    void beforeExpose();
    void exposeLayer(int lay);
    void exposeBgzLayer(int);
    void setCurrentImg(QImage s){currentImg = s;}
    void setBgzSupportImg(QImage s){currentSupportImg = s;}
    void setBgzSupportHeiht(int d){bgzSupportHeight=d;}
    void setBgzSupportMaxHeight(int d){bgzSupportMaxHeight = d;}
    void setTotalLayerNum(int lay){totalLayNum = lay;}


    void closeLight();
    void openLight();
    void cDelay(int mesc);


    double calcuteMovingTime(int speed,double distance);    //  计算运动时间
private:
    QImage currentImg;
    QImage currentSupportImg;       //Bgz文件时，单独出来的支撑文件
    double pFirstExposeTime;        //底座曝光时间
    double pSupportExposeTime;      //支撑/固定前基层 曝光时间
    double pModelExposeTime;        //正常模型曝光时间


    double layerThickness;
    double slowUpDistance;
    double upDownDistance;
    double zTotalDistance;
    double lightHide;

    double zTotalTime;

    int currentLay;
    int totalLayNum;
    int baseLayNum;
    int dividLayNum;
    int slowUpSpeed;
    int normalSpeed;

    //非保存参数变量
    int bgzSupportHeight;
    int bgzSupportMaxHeight;

    //私有函数
    void Log(QString);



private slots:

    void slotExposeA();          //slc 和 bg文件的曝光开始函数
    void slotExposeBgz();         //bgz文件的曝光开始函数
    void slotExposeBgzSupport();  //bgz曝光第二阶段函数，曝光的是支撑文件
    void slotAfterExpose();

    void slotSlowUp();          //曝光完成，慢速拉升
    void slotNormalUp();        //正常速度上升
    void slotNormalDown();      //正常速度下降

    void slotUpDownAll();

    void sendToArduino(QString);
    void recvFromArduino(QString le);

signals:
    void Sig_NextLayer();
    void Sig_ToFrontEnd(QString);

};

#endif // LAYERJOB_H
