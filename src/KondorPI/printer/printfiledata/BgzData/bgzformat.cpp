#include "bgzformat.h"

BGZFormat::BGZFormat(QObject *parent) :
    QObject(parent)
{
    modelSvg= new SvgReaders();

    QSettings* psetting = new QSettings("Config.ini",QSettings::IniFormat);
    psetting->beginGroup("Config");
    project_x = psetting->value("g_x").toInt();
    project_y = psetting->value("g_y").toInt();
    pXYpicel = psetting->value("pixcel").toDouble();
    psetting->endGroup();
}


void BGZFormat::importSvg(QString file)
{
    modelSvg->importSvg_firstPass(file);
}
void BGZFormat::setPixcelPerMM(double m)
{
    pXYpicel = m;
}
void BGZFormat::resetCount()
{
    modelSvg->resetLayer();
}

void BGZFormat::loadPreview(QString file)
{
    preview_img.load(file);
}

QImage BGZFormat::getSvgLayerImage(int layNr)
{
    return modelSvg->importSlicesFromSvg(pXYpicel,layNr);
}

int BGZFormat::getTotalLayer()
{
    return (modelSvg->getSvgTotalLayers());
}
