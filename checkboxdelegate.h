#ifndef CCHECKBOXDELEGATE_H
#define CCHECKBOXDELEGATE_H

#include <QStyledItemDelegate>

class CCheckBoxDelegate : public QStyledItemDelegate
{
public:
    CCheckBoxDelegate(QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CCHECKBOXDELEGATE_H
