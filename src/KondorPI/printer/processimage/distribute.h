#ifndef DISTRIBUTE_H
#define DISTRIBUTE_H

#include <QRgb>
#include <QImage>
#include <QObject>
#include <QSettings>

class Distribute : public QObject
{
    Q_OBJECT
public:
    explicit Distribute(QObject *parent = 0);

    QImage setGrayScale(QImage s);

private:
    int grayMaskArray[4][4];
    int project_x;


};

#endif // DISTRIBUTE_H
