#ifndef CANDY_H
#define CANDY_H

#include <QRgb>
#include <QImage>
#include <QColor>
#include <QObject>

class Candy : public QObject
{
    Q_OBJECT
public:
    explicit Candy(QObject *parent = 0);
    QImage createToverMap3(QImage src);
signals:

public slots:

};

#endif // CANDY_H
