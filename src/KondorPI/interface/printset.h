#ifndef PRINTSET_H
#define PRINTSET_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "printer/printer.h"
#include "interface/storagetype.h"
#include "interface/usbstoragefile.h"
#include "printer/printfiledata/printfiledata.h"
#include "printer/hardware/projector/projector.h"

namespace Ui {
class PrintSet;
}

enum PrintControlStatus
{
    PRINT_IDLE=0,
    PRINT_PRINTING=1,
    PRINT_PAUSE=2,
};

class PrintSet : public QWidget
{
    Q_OBJECT

public:
    explicit PrintSet(QWidget *parent = nullptr);
    ~PrintSet();
    void SetPrintPersent(int persent);
    void Delay(int mesc);

public slots:
    void SlotGenPreview();
    void on_btnStart_clicked();
    void on_btnRecovery_clicked();
    void on_btnCancle_clicked();

    void CmdFinishedSlot(QNetworkReply *reply);
    void SlotUpDown();
    void SlotExposeImg();
    void SlotPrintPreview(QImage src);
    bool removeDirWithContent(QString dirName);

private slots:
    void on_btnBack_clicked();

    void on_btnLoadFile_clicked();

    void on_btnUSBFile_clicked();

private:
    Ui::PrintSet *ui;
    QNetworkAccessManager* gcode_cmd;
    StorageType*st_type;
    USBStorageFile* usb_type;
    PrintFileData* print_data;
    Printer* lcd_printer;

    PrintControlStatus printer_status;
    int current_layer;
    double up_down_action;

    void LoadPrintData();

signals:
    void SigControlStatus(PrintControlStatus);
};

#endif // PRINTSET_H
