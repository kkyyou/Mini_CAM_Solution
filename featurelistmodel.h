#ifndef CFEATURELISTMODEL_H
#define CFEATURELISTMODEL_H

#include <QAbstractTableModel>
#include "feature.h"

class CLayer;
class CFeatureListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CFeatureListModel(QObject *parent = 0);

    enum {_COLUMN_FEATURE_TYPE, _COLUMN_SHAPE_TYPE, _COLUMN_POINT, _COLUMN_SIZE, _COLUMN_COUNT};

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    QString getFeatureTypeString(const FEATURE_TYPE &featureType) const;
    QString getShapeTypeString(const SHAPE_TYPE &shapeType) const;

    void setFeatureList(const QList<CFeature *> &featureList);

public slots:
    void repaint();
    void setActiveLayerSlot(CLayer *activeLayer);
private:
    CLayer *m_activeLayer;
    QList<CFeature *> m_featureList;
};

#endif // CFEATURELISTMODEL_H
