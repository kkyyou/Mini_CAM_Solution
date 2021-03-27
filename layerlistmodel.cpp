#include "layerlistmodel.h"

#include "layer.h"
#include "job.h"

CLayerListModel::CLayerListModel(CJob *job, QObject *parent) :
    QAbstractTableModel(parent),
    m_job(job)
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
        case _COLUMN_NAME:      return layer->layerName();
        default:                break;
        }
    }
    else if (role == Qt::CheckStateRole)
    {
        switch (col)
        {
        case _COLUMN_ACTIVE:
        {
            if (layer->isActive())   return Qt::Checked;
            else                     return Qt::Unchecked;
        }
        case _COLUMN_VIEW:
        {
            if (layer->isView())     return Qt::Checked;
            else                     return Qt::Unchecked;
        }
        default:                break;
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
       return Qt::AlignCenter;
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

Qt::ItemFlags CLayerListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    // 레이어 이름은 VIEW에서 변경 불가.
    if (index.column() == _COLUMN_NAME)
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool CLayerListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_job)
        return false;

    if(!index.isValid())
        return false;

    int row = index.row();
    int col = index.column();

    if (row >= m_layerList.count())
        return false;

    CLayer *layer = m_layerList.at(row);
    if (!layer)
        return false;

    if (role == Qt::CheckStateRole)
    {
        switch (col)
        {
        case _COLUMN_ACTIVE:
        {
            if ((Qt::CheckState)value.toInt() == Qt::Checked)
            {
                // ActiveLayer는 하나만 존재하므로.
                // 이미 ActiveLayer가 존재하면 Active를 지운 후 다시 Active 셋팅 해준다.
                bool existActiveLayer = m_job->existActiveLayer();
                if (existActiveLayer)
                {
                    CLayer *unActiveLayer = m_job->unActiveLayer();
                    unActiveLayer->setIsActive(false);
                }



                if (!layer->isView())
                {
                    // 색상 지정.
                    QQueue<QPair<CLayer *, QColor>> *layerColor = m_job->getLayerAndColorQueue();
                    QPair<CLayer *, QColor> dequeueLC = layerColor->dequeue();
                    QColor dequeueC = dequeueLC.second;
                    layer->setFeatureColor(dequeueC);
                    dequeueLC.first = layer;

                    layerColor->enqueue(dequeueLC);
                }

                // Active 레이어는 당연히 View도 되어야 한다.
                layer->setIsActive(true);
                layer->setIsView(true);

                repaint();
                emit changeActiveLayer(layer);
            }
            else
            {
                layer->setIsActive(false);
            }
        } break;
        case _COLUMN_VIEW:
        {
            if ((Qt::CheckState)value.toInt() == Qt::Checked)
            {
                // 모든 레이어 색상이 사용중인 경우.
                if (m_job->isUsingAllLayerColor())
                {
                    // 색상 지정.
                    QQueue<QPair<CLayer *, QColor>> *layerColor = m_job->getLayerAndColorQueue();
                    QPair<CLayer *, QColor> dequeueLC = layerColor->dequeue();
                    CLayer *dequeueL = dequeueLC.first;
                    if (dequeueL)
                        dequeueL->setIsView(false);
                    QColor dequeueC = dequeueLC.second;
                    layer->setFeatureColor(dequeueC);
                    dequeueLC.first = layer;
                    layerColor->enqueue(dequeueLC);
                }
                else
                {
                    // 색상 지정.
                    QQueue<QPair<CLayer *, QColor>> *layerColor = m_job->getLayerAndColorQueue();
                    QPair<CLayer *, QColor> dequeueLC = layerColor->dequeue();
                    CLayer *dequeueL = dequeueLC.first;
                    QColor dequeueC = dequeueLC.second;
                    layer->setFeatureColor(dequeueC);
                    dequeueLC.first = layer;

                    layerColor->enqueue(dequeueLC);
                }

                layer->setIsView(true);
                repaint();
            }
            else
            {
                if (layer->isActive())
                    return false;

                layer->setIsView(false);
            }
        } break;
        }
    }

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

void CLayerListModel::repaint()
{
    QModelIndex topLeft = index(0,0);
    QModelIndex botRight = index(rowCount() - 1, columnCount() - 1);

    emit dataChanged(topLeft, botRight);
}
