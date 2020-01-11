#ifndef SVGREADERS_H
#define SVGREADERS_H

#include <QObject>
#include <QSvgRenderer>
#include <QImage>
#include <QFile>
#include <QTextStream>
#include <QPainter>

class SvgReaders : public QObject
{
    Q_OBJECT
public:
    explicit SvgReaders(QObject *parent = 0);
    void importSvg_firstPass(QString file);
    QImage importSlicesFromSvg(double pixelsizemicrons,int layNu);

    //getter
    double getScalPix(){return scalPixcel;}
    int getSvgTotalLayers(){return total_layers;}


    //setter
    void setPixcel(double x){pixcel = x;}
    void resetLayer(){total_layers = 0;layers = 0;}
signals:
    void Sig_FileError(int);

public slots:

private:
    int layers;
    int total_layers;
    double xsizemm;
    double ysizemm;

    double scalPixcel;
    double pixcel;


    QSvgRenderer SvgRender;

};

#endif // SVGREADERS_H
