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
        long long diameter = ui->width_line_edit->text().toDouble() * _MILLION;
        emit insertCommandValueSignal(_DIAMETER, diameter);
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

void CAddPadDialog::on_round_radio_btn_clicked()
{
    ui->width_lbl->setText("Diameter : ");

    ui->height_lbl->setVisible(false);
    ui->height_line_edit->setVisible(false);
}

void CAddPadDialog::on_rect_radio_btn_clicked()
{
    ui->width_lbl->setText("Width : ");

    ui->height_lbl->setVisible(true);
    ui->height_line_edit->setVisible(true);
}

void CAddPadDialog::on_close_btn_clicked()
{
    delete this;
}
