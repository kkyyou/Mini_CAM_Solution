#ifndef CLAYERLISTMODEL_H
#define CLAYERLISTMODEL_H

#include <QAbstractTableModel>

class CLayer;
class CLayerListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CLayerListModel(QObject *parent = 0);

    enum {_COLUMN_ACTIVE, _COLUMN_VIEW, _COLUMN_NAME, _COLUMN_COUNT};

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);



    QList<CLayer *> layerList() const;
    void setLayerList(const QList<CLayer *> &layerList);

private:
    QList<CLayer *> m_layerList;


};

#endif // CLAYERLISTMODEL_H
