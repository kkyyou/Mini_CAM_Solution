#include "featurelistmodel.h"
#include "layer.h"

CFeatureListModel::CFeatureListModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_activeLayer(NULL)
{

}

int CFeatureListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_featureList.count();
}

int CFeatureListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _COLUMN_COUNT;
}

QVariant CFeatureListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())                     return QVariant();
    if (index.row() >= m_featureList.count()) return QVariant();
    if (!m_activeLayer)                       return QVariant();

    int row = index.row();
    int col = index.column();

    QList<CFeature *> featureList = m_activeLayer->featureList();

    // Index에 해당하는 Layer 가져오기.
    CFeature *feature = featureList.at(row);
    if (!feature)
        return QVariant();

    CShape *shape = feature->shape();
    if (!shape)
        return QVariant();

    // 칼럼 정보 보여주기.
    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case _COLUMN_FEATURE_TYPE:    return feature->getFeatureTypeString();
        case _COLUMN_SHAPE_TYPE:      return feature->getShapeTypeString();
        case _COLUMN_POINT:           return feature->getPointInfo();
        case _COLUMN_SIZE:            return feature->getSizeInfo();
        default:                      break;
        }
    }
    else if (role == Qt::TextAlignmentRole)     return Qt::AlignCenter;

    return QVariant();
}

QVariant CFeatureListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            // 칼럼 헤더에 칼럼명 표시.
            switch (section)
            {
            case _COLUMN_FEATURE_TYPE:   return QObject::trUtf8("F Type");
            case _COLUMN_SHAPE_TYPE:     return QObject::trUtf8("S Type");
            case _COLUMN_POINT:          return QObject::trUtf8("Point Info");
            case _COLUMN_SIZE:           return QObject::trUtf8("Size Info");
            default:                     break;
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

bool CFeatureListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (m_featureList.isEmpty())  return false;
    if (count < 1)              return false;

    beginInsertRows(parent, row, row+count-1);
    endInsertRows();

    return true;
}

bool CFeatureListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (m_featureList.isEmpty())  return false;
    if (count < 1)              return false;

    beginRemoveRows(parent, row, row+count-1);
    endRemoveRows();

    return true;
}

void CFeatureListModel::setFeatureList(const QList<CFeature *> &featureList)
{
    // 기존 값들 클리어.
    removeRows(0, m_featureList.count(), QModelIndex());
    m_featureList.clear();

    // 새로운 Layer List로 셋팅.
    m_featureList = featureList;

    // 새로운 Layer List 카운트 만큼 로우 추가.
    if (m_featureList.isEmpty())
        return;

    insertRows(0, m_featureList.count(), QModelIndex());
}

void CFeatureListModel::setActiveLayer(CLayer *layer)
{
    m_activeLayer = layer;
}

void CFeatureListModel::repaint()
{
    QModelIndex topLeft = index(0,0);
    QModelIndex botRight = index(rowCount() - 1, columnCount() - 1);

    emit dataChanged(topLeft, botRight);
}

void CFeatureListModel::setActiveLayerSlot(CLayer *activeLayer)
{
    if (!activeLayer)
        return;

    m_activeLayer = activeLayer;

    setFeatureList(activeLayer->featureList());
}
