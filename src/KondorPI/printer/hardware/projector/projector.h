#ifndef PROJECTOR_H
#define PROJECTOR_H

#include <QWidget>
#include <QVector2D>
#include <QSvgRenderer>
#include <QDesktopWidget>
#include <QImage>
#include <QColor>
#include <QFile>
#include <math.h>
#include <QSettings>
#include <QMessageBox>
#include <QMap>
#include <QMutex>
#include <QScopedPointer>

#include <QMainWindow>

#include "dlpcontrol.h"



namespace Ui {
class Projector;
}

class Projector : public QWidget
{
    Q_OBJECT

public:
    static Projector& getInstance();
    void connectProject();
    void showImage(QImage img);
    void showBlank();
    void showBase();

private:
    Ui::Projector *ui;
    explicit Projector(QWidget *parent = 0);
    ~Projector();

    Projector(const Projector &other);
    Projector& operator=(const Projector &other);

    static QMutex mutex;
    static QScopedPointer<Projector>instance;
    friend struct QScopedPointerDeleter<Projector>;

    QImage bank;
    QDesktopWidget* m_pDesktop;
//    ProcessImage* imgEffect;
    DLPcontrol* frameBuffer;

    int project_x;
    int project_y;

    void writeFrameBuffer(QImage src);
    QImage combineFull(QImage draw);

signals:
    void Sig_Img(QImage);

};

#endif // PROJECTOR_H
