#ifndef CRADIOBUTTONDELEGATE_H
#define CRADIOBUTTONDELEGATE_H

#include <QItemDelegate>

class CRadioButtonDelegate : public QItemDelegate
{
public:
    CRadioButtonDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // CRADIOBUTTONDELEGATE_H
