#include "layerlistmodel.h"

#include "layer.h"

CLayerListModel::CLayerListModel(QObject *parent)
{

}

int CLayerListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_layerList.count();
}

int CLayerListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _COLUMN_COUNT;
}

QVariant CLayerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())                   return QVariant();
    if (index.row() >= m_layerList.count()) return QVariant();

    int row = index.row();
    int col = index.column();

    // Index에 해당하는 Layer 가져오기.
    CLayer *layer = m_layerList.at(row);
    if (!layer)
        return QVariant();

    // 칼럼 정보 보여주기.
    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case _COLUMN_ACTIVE:    return QVariant();
        case _COLUMN_VIEW:      return QVariant();
        case _COLUMN_NAME:      return layer->layerName();
        default:                break;
        }
    }

    return QVariant();
}

QVariant CLayerListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            // 칼럼 헤더에 칼럼명 표시.
            switch (section)
            {
            case _COLUMN_ACTIVE:        return QObject::trUtf8("Active");
            case _COLUMN_VIEW:          return QObject::trUtf8("View");
            case _COLUMN_NAME:          return QObject::trUtf8("Name");
            default:                    break;
            }
        }
        else if (orientation == Qt::Vertical)
        {
            // 로우 헤더에 넘버 표시.
            return section + 1;
        }
    }

    return QVariant();
}

bool CLayerListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (m_layerList.isEmpty())  return false;
    if (count < 1)              return false;

    beginInsertRows(parent, row, row+count-1);
    endInsertRows();

    return true;
}

bool CLayerListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (m_layerList.isEmpty())  return false;
    if (count < 1)              return false;

    beginRemoveRows(parent, row, row+count-1);
    endRemoveRows();

    return true;
}

QList<CLayer *> CLayerListModel::layerList() const
{
    return m_layerList;
}

void CLayerListModel::setLayerList(const QList<CLayer *> &layerList)
{
    // 기존 값들 클리어.
    removeRows(0, m_layerList.count(), QModelIndex());
    m_layerList.clear();

    // 새로운 Layer List로 셋팅.
    m_layerList = layerList;

    // 새로운 Layer List 카운트 만큼 로우 추가.
    if (!m_layerList.isEmpty())
        insertRows(0, m_layerList.count(), QModelIndex());

}
