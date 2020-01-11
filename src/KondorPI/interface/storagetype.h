#ifndef STORAGETYPE_H
#define STORAGETYPE_H

#include <QWidget>



namespace Ui {
class StorageType;
}

class StorageType : public QWidget
{
    Q_OBJECT

public:
    explicit StorageType(QWidget *parent = nullptr);
    ~StorageType();

    void UpdateTree();

private slots:


    void on_btnWifiOK_clicked();

    void on_btnBack_clicked();

    void on_btnDelete_clicked();

private:
    Ui::StorageType *ui;

signals:
    void SigStorageFile(QString);
    void SigPreview();
};

#endif // STORAGETYPE_H
