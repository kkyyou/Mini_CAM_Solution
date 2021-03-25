#include "checkboxdelegate.h"

#include <QApplication>
#include <QMouseEvent>

CCheckBoxDelegate::CCheckBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

QWidget *CCheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);

    return NULL;
}

void CCheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 체크 박스의 위치를 중앙으로 맞추기.
    QStyleOptionViewItemV4 viewItemOption(option);

    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
    QRect newRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                                        QSize(option.decorationSize.width() + 5,option.decorationSize.height()),
                                        QRect(option.rect.x() + textMargin, option.rect.y(),
                                              option.rect.width() - (2 * textMargin), option.rect.height()));
    viewItemOption.rect = newRect;

    QStyledItemDelegate::paint(painter, viewItemOption, index);
}

bool CCheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_ASSERT(event);
    Q_ASSERT(model);

    // 체크 가능하고 수정 가능한 Flag인지 체크.
    Qt::ItemFlags flags = model->flags(index);
    if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
        return false;

    // CheckStateRole이 존재하는지 체크.
    QVariant value = index.data(Qt::CheckStateRole);
    if (!value.isValid())
        return false;

    // 마우스 클릭 시 범위에 포함되는지 체크.
    if (event->type() == QEvent::MouseButtonRelease)
    {
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                                              option.decorationSize,
                                              QRect(option.rect.x() + (2 * textMargin), option.rect.y(),
                                                    option.rect.width() - (2 * textMargin),
                                                    option.rect.height()));

        if (!checkRect.contains(static_cast<QMouseEvent*>(event)->pos()))
            return false;
    }
    else    return false;

    // 체크상태를 모델로 넘김.
    Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    return model->setData(index, state, Qt::CheckStateRole);
}

void CCheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 체크박스의 맞춰진 위치로 이동.
    editor->setGeometry(option.rect);
}
