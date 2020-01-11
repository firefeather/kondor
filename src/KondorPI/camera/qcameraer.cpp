#include "qcameraer.h"


QMutex QCameraer::mutex;
QScopedPointer<QCameraer> QCameraer::instance;
const int freq_frame = 33;

QCameraer::QCameraer(QObject *parent) : QObject(parent)
{
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(SlotTimeOut()));
    id = 0;
}

QCameraer& QCameraer::getInstance()
{
    if(instance.isNull())
    {
        mutex.lock();
        if(instance.isNull())
        {
            instance.reset(new QCameraer());
        }
        mutex.unlock();
    }
    return *instance.data();
}

/*********************公开接口开始*****************************/
bool QCameraer::OpenCamera(int ids)
{
    id = ids;
    videoCapture_.open(id);
    if (videoCapture_.isOpened())
    {
        return true;
    }
    else
        return false;
}

void QCameraer::SetWidth(double value)
{
    videoCapture_.set(CV_CAP_PROP_FRAME_WIDTH, value);
}
void QCameraer::SetHeight(double value)
{
    videoCapture_.set(CV_CAP_PROP_FRAME_HEIGHT, value);
}
void QCameraer:: SetBrightness(double value)
{
    videoCapture_.set(CV_CAP_PROP_BRIGHTNESS, value);

}
void QCameraer:: SetContrast(double value)
{
    videoCapture_.set(CV_CAP_PROP_CONTRAST, value);
}
void QCameraer:: SetSaturation(double value)
{
    videoCapture_.set(CV_CAP_PROP_SATURATION, value);
}
void QCameraer:: SetExpose(double value)
{
    videoCapture_.set(CV_CAP_PROP_EXPOSURE, value);
}

void QCameraer::SetAutoExpose()
{
    videoCapture_.set(CV_CAP_PROP_AUTO_EXPOSURE,0.0);
}


double QCameraer::GetWidth()
{
    return videoCapture_.get(CV_CAP_PROP_FRAME_WIDTH);
}
double QCameraer::GetHeight()
{
    return videoCapture_.get(CV_CAP_PROP_FRAME_HEIGHT);
}
double QCameraer:: GetBrightness()
{
    return videoCapture_.get(CV_CAP_PROP_BRIGHTNESS);
}
double QCameraer:: GetContrast()
{
    return videoCapture_.get(CV_CAP_PROP_CONTRAST);
}
double QCameraer:: GetStauration()
{
    return videoCapture_.get(CV_CAP_PROP_SATURATION);
}
double QCameraer:: GetExpose()
{
    return videoCapture_.get(CV_CAP_PROP_EXPOSURE);
}

void QCameraer::CapatureOnce(Mat &output)
{
    cv::Mat capture;
    if (!videoCapture_.read(capture))
    {
        timer->stop();
    }
    capture.copyTo(output);

}


void QCameraer::StartLive()
{
    timer->start(freq_frame);
}
void QCameraer::StopLive()
{
    timer->stop();
}
void QCameraer::CloseCamera()
{
    timer->stop();
}
/***********************公开接口结束***************************/


void QCameraer::SlotTimeOut()
{
    timer->stop();
    cv::Mat capture;
    if (!videoCapture_.read(capture)) // Blocks until a new frame is ready
    {
        timer->stop();
    }

    emit SigLiveCamera(capture);

    timer->start(freq_frame);
}
