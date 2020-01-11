#include "fisheye.h"

using namespace QtOcv;

FishEye::FishEye(QObject *parent) :
    QObject(parent)
{

}

/**	Function to calculate shift*/
float FishEye::calc_shift(float x1, float x2, float cx, float k)
{
//    float thresh = 1;
//    float x3 = x1 + (x2 - x1)*0.5;
//    float res1 = x1 + ((x1 - cx)*k*((x1 - cx)*(x1 - cx)));
//    float res3 = x3 + ((x3 - cx)*k*((x3 - cx)*(x3 - cx)));

//    if (res1>-thresh && res1 < thresh)
//        return x1;
//    if (res3<0)
//    {
//        return calc_shift(x3, x2, cx, k);
//    }
//    else
//    {
//        return calc_shift(x1, x3, cx, k);
//    }
}

float FishEye::getRadialX(float x, float y, float cx, float cy, float k, bool scale, Vec4f props)
{
//    float result;
//    if (scale)
//    {
//        float xshift = props[0];
//        float yshift = props[1];
//        float xscale = props[2];
//        float yscale = props[3];

//        x = (x*xscale + xshift);
//        y = (y*yscale + yshift);
//        result = x + ((x - cx)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
//    }
//    else
//    {
//        result = x + ((x - cx)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
//    }
//    return result;
}

float FishEye::getRadialY(float x, float y, float cx, float cy, float k, bool scale, Vec4f props)
{
//    float result;
//    if (scale)
//    {
//        float xshift = props[0];
//        float yshift = props[1];
//        float xscale = props[2];
//        float yscale = props[3];

//        x = (x*xscale + xshift);
//        y = (y*yscale + yshift);
//        result = y + ((y - cy)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
//    }
//    else
//    {
//        result = y + ((y - cy)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
//    }
//    return result;
}

//	Fish Eye Function							Cx, Cy Center of x & y
//	_src : Input image, _dst : Output image, Cx,Cy coordinates from where the distorted image will have as initial point, k : distortion factor
void FishEye::fishEye(InputArray _src, OutputArray _dst, double Cx, double Cy, double kx, bool scale)
{
//    scale = false;
//    Mat src = _src.getMat();
//    Mat mapx = Mat(src.size(), CV_32FC1);
//    Mat mapy = Mat(src.size(), CV_32FC1);

//    int w = src.cols;	//	Width
//    int h = src.rows;	//	Height

//    Vec4f props;
//    //Calculating x and y shifts to be applied
//    float xShift = calc_shift(0, Cx - 1, Cx, kx);
//    props[0] = xShift;
//    float newCenterX = w - Cx;
//    float xShift2 = calc_shift(0, newCenterX - 1, newCenterX, kx);

//    float yShift = calc_shift(0, Cy - 1, Cy, kx);
//    props[1] = yShift;
//    float newCenterY = w - Cy;
//    float yShift2 = calc_shift(0, newCenterY - 1, newCenterY, kx);

//    //	Calculating the scale factor from the x & y shifts accordingly
//    float xScale = (w - xShift - xShift2) / w;
//    props[2] = xScale;
//    float yScale = (h - yShift - yShift2) / h;
//    props[3] = yScale;

//    float* p = mapx.ptr<float>(0);

//    for (int y = 0; y < h; y++)
//    {
//        for (int x = 0; x < w; x++)
//        {
//            *p++ = getRadialX((float)x, (float)y, Cx, Cy, kx, scale, props);
//        }
//    }

//    p = mapy.ptr<float>(0);

//    for (int y = 0; y < h; y++)
//    {
//        for (int x = 0; x < w; x++)
//        {
//            *p++ = getRadialY((float)x, (float)y, Cx, Cy, kx, scale, props);
//        }
//    }

//    remap(src, _dst, mapx, mapy, CV_INTER_LINEAR, BORDER_CONSTANT);

}

QImage FishEye::getFishEye(QImage s)
{
//    Mat in = image2Mat(s);
//    Mat out;
//    QSettings settings("Config.ini",QSettings::IniFormat);
//    settings.beginGroup("keystone_point");
//    int fishValue = (settings.value("fisheye").toInt());
//    settings.endGroup();

//    double ks = fishValue*0.000000001;
//    fishEye(in, out, in.cols / 2, in.rows / 2, ks,false);
//    QImage result = mat2Image(out);
//    return result;
}

QImage FishEye::getFishEyeShow(QImage s,int fishValue)
{
//    Mat in = image2Mat(s);
//    Mat out;


//    double ks = fishValue*0.000000001;
//    fishEye(in, out, in.cols / 2, in.rows / 2, ks,false);
//    QImage result = mat2Image(out);
//    return result;
}
