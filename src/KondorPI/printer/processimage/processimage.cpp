#include "processimage.h"

ProcessImage::ProcessImage(QObject *parent) :
    QObject(parent)
{
    cand = new Candy();
    disbute = new Distribute();
    eye = new FishEye();
    keynot = new Keystone();

}

QImage ProcessImage::process(QImage s)
{
//    QImage a = keynot->keyNote(s);
//    QImage b = eye->getFishEye(a);
//    QImage c = disbute->setGrayScale(s);
    return s;
}
