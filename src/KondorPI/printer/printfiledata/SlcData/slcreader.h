#ifndef SLCREADER_H
#define SLCREADER_H

#include <QFile>
#include <QDebug>
#include <QObject>
#include <QPainter>
#include <QVector2D>
#include <QTextStream>
#include <QSettings>

#include <math.h>

class SlcReader : public QObject
{
    Q_OBJECT
public:
    explicit SlcReader(QObject *parent = 0);
    void importSlc_firstPass(QString file);
    QImage importSlicesFromSlc(QString file,double pixelsizemicrons = 100.0,int layerNu=0);
    int getSlcTotalLayers(){return total_layers;}
    double getThickness(){return thickness;}
public slots:
private:
    //slc file's
    QVector2D maxb;
    QVector2D minb;
    qint64 returnpos;
    double thickness;
    double scalefactor;
    int total_layers;

signals:



};

#endif // SLCREADER_H
