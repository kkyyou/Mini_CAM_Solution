#include "addpaddialog.h"
#include "ui_addpaddialog.h"
#include "typedefine.h"

CAddPadDialog::CAddPadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddPadDialog)
{
    ui->setupUi(this);
}

CAddPadDialog::~CAddPadDialog()
{
    delete ui;
}

void CAddPadDialog::on_setting_btn_clicked()
{
    if (ui->round_radio_btn->isChecked())
    {
        long long radius = ui->width_line_edit->text().toDouble() * _MILLION;
        emit insertCommandValueSignal(_RADIUS, radius);
        emit updateCurrentCommandSignal(_ADD_PAD, _ROUND);
    }
    else if (ui->rect_radio_btn->isChecked())
    {
        long long width = ui->width_line_edit->text().toDouble() * _MILLION;
        long long height = ui->height_line_edit->text().toDouble() * _MILLION;
        emit insertCommandValueSignal(_WIDTH, width);
        emit insertCommandValueSignal(_HEIGHT, height);
        emit updateCurrentCommandSignal(_ADD_PAD, _RECTANGLE);
    }
}
