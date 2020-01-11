#ifndef PRINTJOB_H
#define PRINTJOB_H

#include <QObject>
#include <QSettings>


#include "layerjob.h"
#include "../PrintFileData/printfiledata.h"

class PrintJob : public QObject
{
    Q_OBJECT
public:
    explicit PrintJob(QObject *parent = 0);
    void loadFile(QString file);
    void reset();
    void startCycle();
    void pauseJob();
    void recovertJob();
    void cancleJob();
    void connectProjector();

    PrintFileData* printdata;
private:
    int currentLayerCount;
    int totalLayer;
    int startLayer;

    int bgzSupportHeight;
    int bgzSupportMaxHeight;
    bool bStop;
    bool bBgz;

    LayerJob* layer;
    void Log(QString s);
    void sendToArduino(QString s);

public slots:
    void slotNextLayer();

signals:
    void Sig_PrintFinish();


};

#endif // PRINTJOB_H
