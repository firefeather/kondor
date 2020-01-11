#ifndef QCAMERAER_H
#define QCAMERAER_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QSettings>
#include <QScopedPointer>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>



using namespace cv;
using namespace std;

class QCameraer : public QObject
{
    Q_OBJECT
public:
    static QCameraer& getInstance();
    bool OpenCamera(int id);

    void SetBrightness(double value);
    void SetContrast(double value);
    void SetSaturation(double value);
    void SetExpose(double value);
    void SetAutoExpose();
    void SetHeight(double value);
    void SetWidth(double value);


    double GetBrightness();
    double GetContrast();
    double GetStauration();
    double GetExpose();
    double GetWidth();
    double GetHeight();


    void CapatureOnce(Mat &output);

    void StartLive();
    void StopLive();
    void CloseCamera();


private:
    QCameraer(QObject *parent = nullptr);
    QCameraer& operator=(const QCameraer &other);

    static QMutex mutex;
    static QScopedPointer<QCameraer>instance;
    friend struct QScopedPointerDeleter<QCameraer>;
    cv::VideoCapture videoCapture_;
    QTimer* timer;

    int id;



signals:
    void SigLiveCamera(Mat t);

private slots:
    void SlotTimeOut();


};

#endif // QCAMERAER_H
