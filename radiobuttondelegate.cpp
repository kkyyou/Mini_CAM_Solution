#include "radiobuttondelegate.h"

#include <QRadioButton>
#include <QDebug>

CRadioButtonDelegate::CRadioButtonDelegate()
{

}

QWidget *CRadioButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QRadioButton *editor = new QRadioButton(parent);
    editor->setAutoFillBackground(true);
    //connect(editor, SIGNAL(toggled(bool)), this, SLOT(emitCommitData()));
    return editor;
}

void CRadioButtonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.data().value<bool>())
    {
        QRadioButton *radio = static_cast<QRadioButton*>(editor);
        bool value = index.data().toBool();
        radio->setChecked(value);
    }
}

void CRadioButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(model);
    QRadioButton *radio = static_cast<QRadioButton *>(editor);
    qDebug() << radio->isChecked(); // returns true and false per click
    model->setData(index, radio->isChecked());
}
