#ifndef ADDLINEDIALOG_H
#define ADDLINEDIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class CAddLineDialog;
}

class CAddLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAddLineDialog(QWidget *parent = nullptr);
    ~CAddLineDialog();

signals:
    void insertCommandValueSignal(QString, QVariant);
    void updateCurrentCommandSignal(QString, QString);

private slots:
    void on_setting_btn_clicked();

    void on_close_btn_clicked();

private:
    Ui::CAddLineDialog *ui;
};

#endif // ADDLINEDIALOG_H
