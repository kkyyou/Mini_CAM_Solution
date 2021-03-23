#include "addfeaturedialog.h"
#include "ui_addfeaturedialog.h"
#include "typedefine.h"

CAddFeatureDialog::CAddFeatureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddFeatureDialog)
{
    ui->setupUi(this);
}

CAddFeatureDialog::~CAddFeatureDialog()
{
    delete ui;
}

void CAddFeatureDialog::on_setting_btn_2_clicked()
{
    QString command = windowTitle();
    if (ui->round_radio_btn_2->isChecked())
    {
        long long radius = ui->size_line_edit_2->text().toDouble() * _MILLION;
        emit insertCommandValueSignal(_RADIUS, radius);
        emit updateCurrentCommand(command, _ROUND);
    }
    else if (ui->rect_radio_btn_2->isChecked())
    {
        long long width = ui->size_line_edit_2->text().toDouble() * _MILLION;
        emit insertCommandValueSignal(_RADIUS, radius);
        emit updateCurrentCommand(command, _RECTANGLE);
    }
}
