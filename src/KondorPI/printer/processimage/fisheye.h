#ifndef FISHEYE_H
#define FISHEYE_H

#include <QObject>
#include <QSettings>
#include <opencv2/opencv.hpp>

#include "../ThirdParty/OpencvAPI/cvmatandqimage.h"
using namespace cv;

class FishEye : public QObject
{
    Q_OBJECT
public:
    explicit FishEye(QObject *parent = 0);
    QImage getFishEye(QImage s);
    QImage getFishEyeShow(QImage s,int fishValue);

private:
    void fishEye(InputArray _src, OutputArray _dst, double Cx, double Cy, double kx, bool scale);
    float getRadialY(float x, float y, float cx, float cy, float k, bool scale, Vec4f props);
    float getRadialX(float x, float y, float cx, float cy, float k, bool scale, Vec4f props);
    float calc_shift(float x1, float x2, float cx, float k);

};

#endif // FISHEYE_H
