#include "keystone.h"

using namespace cv;
using namespace QtOcv;

Keystone::Keystone(QObject *parent) :
    QObject(parent)
{
    QSettings settings("Config.ini",QSettings::IniFormat);

    settings.beginGroup("formPrinter");
    project_x = settings.value("g_x").toInt();
    project_y = settings.value("g_y").toInt();
    double pXY = settings.value("pixcel").toDouble();
    settings.endGroup();

    settings.beginGroup("keystone_point");
    bx_1 = settings.value("1x").toDouble();
    by_1 = settings.value("1y").toDouble();
    bx_2 = settings.value("2x").toDouble();
    by_2 = settings.value("2y").toDouble();
    bx_3 = settings.value("3x").toDouble();
    by_3 = settings.value("3y").toDouble();
    bx_4 = settings.value("4x").toDouble();
    by_4 = settings.value("4y").toDouble();
    b_t = settings.value("b_p").toInt();


    if(b_t==1)
    {
        w_top = settings.value("w_top").toDouble();
        w_buttom= settings.value("w_buttom").toDouble();
        h_left= settings.value("h_left").toDouble();
        h_right= settings.value("h_right").toDouble();
        double w1 = 1000.0*(qMax(w_top,w_buttom)/project_x);
        double h1 = 1000.0*(qMax(h_left,h_right)/project_x);
        pXY = qMax(w1,h1);
        pXYpicel = pXY;

    }
    else if(b_t==0)
    {
        pXYpicel = pXY;
    }
    settings.endGroup();
}

/*
 *
 *          1st----------2nd
 *           |             |
 *           |             |
 *           |             |
 *           |             |
 *          3rd----------4th
 *
 *
 */
QImage Keystone::keyNote(QImage src)
{
    cv::Point2f src_vertices[4];
    cv::Point2f dst_vertices[4];
    Mat dst;

    Mat srcImage = image2Mat(src);
    int m1_x = 0;
    int m1_y = 0;

    int m2_x = src.width();
    int m2_y = 0;

    int m3_x = 0;
    int m3_y = src.height();

    int m4_x = src.width();
    int m4_y = src.height();

    src_vertices[0] = Point2f(m1_x, m1_y);
    src_vertices[1] = Point2f(m2_x,m2_y);
    src_vertices[2] = Point2f(m3_x, m3_y);
    src_vertices[3] = Point2f(m4_x,m4_y);

    double g_x = project_x*pXYpicel/1000.0;
    double g_y = project_y*pXYpicel/1000.0;


    double x_1 = bx_1*1000.0/pXYpicel;
    double y_1 = by_1*1000.0/pXYpicel;

    double x_2 = (g_x+bx_2)*1000.0/pXYpicel;
    double y_2 = by_2*1000.0/pXYpicel;

    double x_3 = bx_3*1000.0/pXYpicel;
    double y_3 = (g_y+by_3)*1000.0/pXYpicel;

    double x_4 = (g_x+bx_4)*1000.0/pXYpicel;
    double y_4 = (g_y+by_4)*1000.0/pXYpicel;

    if((x_1<0)||(y_1<0)||(y_2<0)||(x_3<0))
    {
        double mx = qMin(x_1,x_3);
        double my = qMin(y_1,y_2);
        if(mx<0)
        {
            x_1=x_1 - mx;
            x_2=x_2 - mx;
            x_3=x_3 - mx;
            x_4=x_4 - mx;
        }
        if(my<0)
        {
            y_1=y_1-my;
            y_2=y_2-my;
            y_3=y_3-my;
            y_4=y_4-my;
        }
    }

    //pXYpicel
    dst_vertices[0] = Point2f(x_1, y_1);
    dst_vertices[1] = Point2f(x_2, y_2);
    dst_vertices[2] = Point2f(x_3, y_3);
    dst_vertices[3] = Point2f(x_4, y_4);

    Mat warpMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
//    warpMatrix_debug = warpMatrix;
    double s_x = qMax(x_2,x_4);
    double s_y = qMax(y_3,y_4);
    cv::Size size(s_x,s_y);
    warpPerspective(srcImage, dst, warpMatrix, size);
    QImage result = mat2Image(dst);
    result.save("jj.jpg","JPEG",100);
    return result;

}
