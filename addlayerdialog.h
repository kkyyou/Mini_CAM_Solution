#ifndef ADDLAYERDIALOG_H
#define ADDLAYERDIALOG_H

#include <QDialog>

namespace Ui {
class CAddLayerDialog;
}

class CAddLayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAddLayerDialog(QWidget *parent = nullptr);
    ~CAddLayerDialog();

signals:
    void addLayerSignal(QString);

private slots:
    void on_ok_btn_clicked();

    void on_close_btn_clicked();

private:
    Ui::CAddLayerDialog *ui;
};

#endif // ADDLAYERDIALOG_H
