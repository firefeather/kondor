#ifndef BGZFORMAT_H
#define BGZFORMAT_H

#include <QObject>
#include <QSettings>
#include "../SvgData/svgreaders.h"

class BGZFormat : public QObject
{
    Q_OBJECT
public:
    explicit BGZFormat(QObject *parent = 0);

    void importSvg(QString file);
    void loadPreview(QString file);
    void setSvgConfig(int mids,int top,int supports){midSuppotStart = mids;topSuppotStart = top;supportStart = supports;}
    void setPXY(int x,int y){project_x = x;project_y = y;}
    void setPixcelPerMM(double m);

    int getTotalLayer();
    int getSupportStart(){return supportStart;}

    void resetCount();

    QImage getSvgLayerImage(int);

    QImage preview_img;

public slots:

private:
    //重构分离svg的读取
    SvgReaders *modelSvg;

    int midSuppotStart;
    int topSuppotStart;
    int supportStart;

    int project_x;
    int project_y;

    double pXYpicel;

};

#endif // BGZFORMAT_H
