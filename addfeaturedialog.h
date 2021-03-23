#ifndef ADDFEATUREDIALOG_H
#define ADDFEATUREDIALOG_H

#include <QDialog>

namespace Ui {
class CAddFeatureDialog;
}

class CAddFeatureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAddFeatureDialog(QWidget *parent = nullptr);
    ~CAddFeatureDialog();

signals:
    void insertCommandValueSignal(QString, QVariant);
    void updateCurrentCommand(QString, QString);

private slots:
    void on_setting_btn_2_clicked();

private:
    Ui::CAddFeatureDialog *ui;
};

#endif // ADDFEATUREDIALOG_H
