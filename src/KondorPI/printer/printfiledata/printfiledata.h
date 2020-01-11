#ifndef PRINTFILEDATA_H
#define PRINTFILEDATA_H

#include <QObject>
#include <QFileInfo>
#include <QSettings>

#include "./tools/zipunzip/zipunzip.h"

#include "./BgzData/bgzformat.h"
#include "./SlcData/slcreader.h"
#include "./SvgData/svgreaders.h"

class PrintFileData : public QObject
{
    Q_OBJECT
public:
    explicit PrintFileData(QObject *parent = 0);
    void loadPrintFile(QString f);

    double getSlcThickness();
    int getTotalLayers();
    int getBgzSupportHeight(){return bgzSupportHeight;}
    int getBgzSupportMaxHeight(){return bgzSupportMaxHeight;}
    QImage getLayerImage(int);
    QImage getPreviewImage(){return preview_img;}


public slots:

private:
    int fileType;   //0==slc  1==svg 2==bgz
    int project_x;
    int project_y;
    double pixcel;
    QString fileDir;
    BGZFormat* bgzFile;
    SlcReader* slcFile;
    SvgReaders* svgFile;
    ZipUnzip* zip;
    QImage preview_img;


    int bgzSupportHeight;
    int bgzSupportMaxHeight;

    QImage combineFull(QImage draw);
    QPoint imagePos(QImage &image);

};

#endif // PRINTFILEDATA_H
