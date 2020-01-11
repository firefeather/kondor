#include "distribute.h"


Distribute::Distribute(QObject *parent) :
    QObject(parent)
{
    QSettings settings("Config.ini",QSettings::IniFormat);
    settings.beginGroup("formPrinter");
    //投影仪的分辨率
    project_x = settings.value("g_x").toInt();
    settings.endGroup();

    settings.beginGroup("light_scene");
    grayMaskArray[0][0]=settings.value("LA1").toInt();
    grayMaskArray[0][1]=settings.value("LA2").toInt();
    grayMaskArray[0][2]=settings.value("LA3").toInt();
    grayMaskArray[0][3]=settings.value("LA4").toInt();
    grayMaskArray[1][0]=settings.value("LA5").toInt();
    grayMaskArray[1][1]=settings.value("LA6").toInt();
    grayMaskArray[1][2]=settings.value("LA7").toInt();
    grayMaskArray[1][3]=settings.value("LA8").toInt();
    grayMaskArray[2][0]=settings.value("LA9").toInt();
    grayMaskArray[2][1]=settings.value("LA10").toInt();
    grayMaskArray[2][2]=settings.value("LA11").toInt();
    grayMaskArray[2][3]=settings.value("LA12").toInt();

    grayMaskArray[3][0]=settings.value("LA13").toInt();
    grayMaskArray[3][1]=settings.value("LA14").toInt();
    grayMaskArray[3][2]=settings.value("LA15").toInt();
    grayMaskArray[3][3]=settings.value("LA16").toInt();
    settings.endGroup();
}

QImage Distribute::setGrayScale(QImage src1)
{
    QImage src = src1;
    int width = src.width();
    int height = src.height();
    QRgb threadWhite = qRgb(220,220,220);

    QRgb *pixels = (QRgb *)src.scanLine(0);
    int on_y=0;
    int on_x=0;
    if(project_x==1920)
    {
        on_x = 480;
        on_y = 270;
    }
    else if(project_x==1280)
    {
        on_x = 320;
        on_y = 200;
    }

    for (int n = 0; n < height; n++)
    {
        int ys = n/on_y;
        for (int m = 0; m < width; m++)
        {
            int xs = m/on_x;
            QRgb thread = qRgb(grayMaskArray[ys][xs],grayMaskArray[ys][xs],grayMaskArray[ys][xs]);
            if(pixels[m+n*width]>threadWhite)
                pixels[m + n*width] = thread;
        }
    }
    return src;
}






