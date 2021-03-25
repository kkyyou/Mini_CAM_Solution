#ifndef CLAYERLISTMODEL_H
#define CLAYERLISTMODEL_H

#include <QAbstractTableModel>

class CLayer;
class CJob;
class CLayerListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CLayerListModel(CJob *job, QObject *parent = 0);

    enum {_COLUMN_ACTIVE, _COLUMN_VIEW, _COLUMN_NAME, _COLUMN_COUNT};

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QList<CLayer *> layerList() const;
    void setLayerList(const QList<CLayer *> &layerList);

signals:
    void changeActiveLayer(CLayer *activeLayer);

public slots:
    void repaint();

private:
    QList<CLayer *> m_layerList;
    CJob *m_job;
};

#endif // CLAYERLISTMODEL_H
