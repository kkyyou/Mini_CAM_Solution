#include "addlinedialog.h"
#include "ui_addlinedialog.h"
#include "typedefine.h"

CAddLineDialog::CAddLineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddLineDialog)
{
    ui->setupUi(this);
}

CAddLineDialog::~CAddLineDialog()
{
    delete ui;
}

void CAddLineDialog::on_setting_btn_clicked()
{
    if (ui->round_radio_btn->isChecked())
    {
        long long radius = ui->width_line_edit->text().toDouble() * _MILLION;
        emit insertCommandValueSignal(_RADIUS, radius);
        emit updateCurrentCommandSignal(_ADD_LINE, _ROUND);
    }
    else if (ui->rect_radio_btn->isChecked())
    {
        long long width = ui->width_line_edit->text().toDouble() * _MILLION;
        emit insertCommandValueSignal(_WIDTH, width);
        emit updateCurrentCommandSignal(_ADD_LINE, _RECTANGLE);
    }
}
