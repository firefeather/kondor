#include <QApplication>
#include <QDebug>
#include <QSettings>
#include "svgreaders.h"

SvgReaders::SvgReaders(QObject *parent) :
    QObject(parent)
{

    total_layers = 0;
    layers = 0;
    xsizemm = 0.0;
    ysizemm = 0.0;
    scalPixcel = 1.0;
    pixcel = 0;

    QSettings* psetting = new QSettings("Config.ini",QSettings::IniFormat);
    psetting->beginGroup("Config");
    pixcel = psetting->value("pixcel").toDouble();
    psetting->endGroup();
}

void SvgReaders::importSvg_firstPass(QString file)
{
    double x = 0;
    double y = 0;
    bool inverted = false;
    layers = 0;
    //    QString po = "print.svg";

    //    QFile::copy(file,"print.svg");
    if(!SvgRender.load(file))
    {
        printf("Unable to import SVG file.\n");
        emit Sig_FileError(1);
        return;
    }
    if(!SvgRender.elementExists("layer0"))
    {
        emit Sig_FileError(2);
        printf("SVG file does not contain compatible layer information\nUnable to import.\n");
        return;
    }
    qDebug()<<"load svg successful!";
    //do a quick search for the word "slic3r:type="contour"" and then the following "style="fill:"
    //to figure out whether the image is inverted or not.
    QString buff = "";
    QFile searchfile(file);
    searchfile.open(QIODevice::ReadOnly);
    QTextStream searchstream(&searchfile);

    while(buff != "slic3r:type=\"contour\"" && !searchstream.atEnd())
    {
        searchstream >> buff;
    }
    if(!searchstream.atEnd())//we must have found it.
    {
        while(buff != "style=\"fill:" && !searchstream.atEnd())
        {
            searchstream >> buff;
        }
        if(!searchstream.atEnd())
        {
            searchstream >> buff;
            if(buff == "white\"")
            {
                inverted = false;
            }
            else
            {
                inverted = true;
            }
        }
        else
        {
            //printf("Could not determine color scheme\nImage may be inverted.\n");
            return;
        }
    }
    else
    {
        //printf("Could not determine color scheme\nImage may be inverted.\n");
    }
    searchfile.close();
    //END SEARCH SECTION


    //begin looping to figure out how many layers there are in the svg file.
    while(SvgRender.elementExists("layer" + QString().number(layers)))
    {
        x = SvgRender.boundsOnElement("layer" + QString().number(layers)).right();
        y = SvgRender.boundsOnElement("layer" + QString().number(layers)).bottom();

        if(x > xsizemm)
            xsizemm = x;

        if(y > ysizemm)
            ysizemm = y;

        layers++;
        QApplication::processEvents();
    }
    if(xsizemm <= 0 || ysizemm <= 0)
    {
        printf("Bad bounds in SVG file.\n");
        return;
    }
    total_layers = layers;

}

QImage SvgReaders::importSlicesFromSvg(double pixelsizemicrons,int layNu)
{
    //we have to figure out the pixel dimentions of
    //get the pixel size in microns from the user
    double pixelsizemm;
    double layersizemm;

    pixelsizemm = pixelsizemicrons*0.001;
    layersizemm = pixelsizemicrons*0.001;
    QImage img(double(xsizemm)/pixelsizemm,double(ysizemm)/pixelsizemm,QImage::Format_ARGB32);
    qDebug()<<tr("img width = %1").arg(img.width());
    QPainter painter(&img);

    img.fill(Qt::black);
    QRectF realelementbounds = SvgRender.boundsOnElement("layer" + QString().number(layNu));
    QRectF pixelbounds(realelementbounds.left()/pixelsizemm, realelementbounds.top()/pixelsizemm, realelementbounds.width()/pixelsizemm, realelementbounds.height()/pixelsizemm);
    painter.begin(&img);

    if(layNu<layers&&(layNu>0))
    {
        painter.setBrush(QColor(149,0,255));
        SvgRender.render(&painter, "layer" + QString().number(layNu),pixelbounds);
    }
    painter.end();
    img = img.mirrored (false, true);

    scalPixcel = pixelsizemicrons/pixcel;
    QImage showPic = img.scaled(img.width()*scalPixcel,img.height()*scalPixcel,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    if(layNu==1)
    {
        QImage s(img.size(),QImage::Format_ARGB32);
        s.fill(Qt::white);
        s.save("base.jpg","JPG",100);
    }
    return showPic;
}
