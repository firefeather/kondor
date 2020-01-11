#include "printfiledata.h"

PrintFileData::PrintFileData(QObject *parent) :
    QObject(parent)
{
    bgzFile = new BGZFormat();
    slcFile = new SlcReader();
    svgFile = new SvgReaders();
    zip = new ZipUnzip();

    QSettings* psetting = new QSettings("Config.ini",QSettings::IniFormat);
    psetting->beginGroup("Config");
    pixcel = psetting->value("pixcel").toDouble();
    project_x = psetting->value("g_x").toInt();
    project_y = psetting->value("g_y").toInt();
    psetting->endGroup();

    bgzSupportHeight = 0;
    bgzSupportMaxHeight = 0;
}

void PrintFileData::loadPrintFile(QString f)
{
    if(QFileInfo(f).completeSuffix() == "bgz")
    {
        fileType = 2;
        delete bgzFile;
        bgzFile = new BGZFormat();
        zip->unzip(f,"./project");

        //装载四个文件
        bgzFile->importSvg("./project/model.bg");
        bgzFile->loadPreview("./project/preview.jpg");
        preview_img = bgzFile->preview_img;
        fileDir = f;

    }
    else if(QFileInfo(f).completeSuffix() == "slc")
    {
        fileType = 0;
        delete slcFile;
        slcFile = new SlcReader();
        QFile::copy(f,"./printing.slc");
        slcFile->importSlc_firstPass("./printing.slc");
        fileDir = "./printing.slc";
    }


}
double PrintFileData::getSlcThickness()
{
    return  slcFile->getThickness();
}
int PrintFileData::getTotalLayers()
{
    int s = 0;
    switch(fileType)
    {
        case 0:
        {
            s = slcFile->getSlcTotalLayers();
            break;
        }
        case 1:
        {
            s = svgFile->getSvgTotalLayers();
            break;
        }
        case 2:
        {
            s = bgzFile->getTotalLayer();
            break;
        }
    }

    return s;

}

QImage PrintFileData::getLayerImage(int lay)
{
    QImage m;
    switch(fileType)
    {
        case 0:
        {
            m = slcFile->importSlicesFromSlc(fileDir,pixcel,lay);
            break;
        }

        case 1:
        {
            m = svgFile->importSlicesFromSvg(pixcel,lay);
            break;
        }
        case 2:
        {
            m = bgzFile->getSvgLayerImage(lay);
            break;
        }
    }
    return combineFull(m);

}

//将图片都变成满屏像素
QImage PrintFileData::combineFull(QImage draw)
{
    QImage back = QImage(project_x,project_y, QImage::Format_ARGB32_Premultiplied);
    back.fill(Qt::black);
    QPainter painter;
    painter.begin((&back));
    painter.drawImage(imagePos(draw), draw);
    painter.end();

    return back;
}

QPoint PrintFileData::imagePos(QImage &image)
{
    return QPoint((project_x - image.width()) / 2,(project_y - image.height()) / 2);
}
