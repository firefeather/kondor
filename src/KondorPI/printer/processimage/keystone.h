#ifndef KEYSTONE_H
#define KEYSTONE_H

#include <QObject>
#include <QSettings>

//openCV include
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "fisheye.h"

#include "../ThirdParty/OpencvAPI/cvmatandqimage.h"


class Keystone : public QObject
{
    Q_OBJECT
public:
    explicit Keystone(QObject *parent = 0);
    QImage keyNote(QImage src);
private:
    QImage sourceImg;

    int b_t;
    double bx_1;
    double by_1;
    double bx_2;
    double by_2;
    double bx_3;
    double by_3;
    double bx_4;
    double by_4;
    double pXYpicel;

    int project_x;
    int project_y;

    double w_top;
    double w_buttom;
    double h_left;
    double h_right;

};

#endif // KEYSTONE_H
