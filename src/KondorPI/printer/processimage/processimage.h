#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H

#include <QObject>

#include "candy.h"
#include "distribute.h"
#include "fisheye.h"
#include "keystone.h"

class ProcessImage : public QObject
{
    Q_OBJECT
public:
    explicit ProcessImage(QObject *parent = 0);
    QImage process(QImage s);

public slots:

private:
    Candy* cand;
    Distribute* disbute;
    FishEye* eye;
    Keystone* keynot;
};

#endif // PROCESSIMAGE_H
