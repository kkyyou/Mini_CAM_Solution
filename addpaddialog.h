#ifndef ADDPADDIALOG_H
#define ADDPADDIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class CAddPadDialog;
}

class CAddPadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAddPadDialog(QWidget *parent = nullptr);
    ~CAddPadDialog();

signals:
    void insertCommandValueSignal(QString, QVariant);
    void updateCurrentCommandSignal(QString, QString);

private slots:
    void on_setting_btn_clicked();

    void on_round_radio_btn_clicked();

    void on_rect_radio_btn_clicked();

    void on_close_btn_clicked();

private:
    Ui::CAddPadDialog *ui;
};

#endif // ADDPADDIALOG_H
