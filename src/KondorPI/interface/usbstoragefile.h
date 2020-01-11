#ifndef USBSTORAGEFILE_H
#define USBSTORAGEFILE_H

#include <QWidget>

namespace Ui {
class USBStorageFile;
}

class USBStorageFile : public QWidget
{
    Q_OBJECT

public:
    explicit USBStorageFile(QWidget *parent = nullptr);
    ~USBStorageFile();
    void UpdateTree();

private slots:
    void on_btnBack_clicked();

    void on_btnWifiOK_clicked();

private:
    Ui::USBStorageFile *ui;
    QString stdout_str;

signals:
    void SigPreview();
};

#endif // USBSTORAGEFILE_H
