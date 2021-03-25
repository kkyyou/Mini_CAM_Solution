#include "addlayerdialog.h"
#include "ui_addlayerdialog.h"

#include <QMessageBox>

CAddLayerDialog::CAddLayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddLayerDialog)
{
    ui->setupUi(this);
}

CAddLayerDialog::~CAddLayerDialog()
{
    delete ui;
}

void CAddLayerDialog::on_ok_btn_clicked()
{
    QString layerName = ui->lineEdit->text();

    // 레이어 이름이 존재하면 시그널 방출.
    if (!layerName.isEmpty())
    {
        emit addLayerSignal(layerName);
        delete this;
        return;
    }

    // 레이어 이름 입력 안하면 메시지 띄우기.
    QMessageBox::warning(this, "Layer Name Empty", "레이어 이름을 입력하세요.", QMessageBox::Yes);
}

void CAddLayerDialog::on_close_btn_clicked()
{
    delete this;
}
