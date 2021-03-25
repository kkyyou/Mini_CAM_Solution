#ifndef CLAYER_H
#define CLAYER_H

#include <QObject>
#include <QList>

class CFeature;
class CLayer
{
public:
    CLayer();

    void appendFeature(CFeature *feature);

    QList<CFeature *> featureList() const;
    void setFeatureList(const QList<CFeature *> &featureList);

    QString layerName() const;
    void setLayerName(const QString &layerName);

private:
    QString m_layerName;

    QList<CFeature *> m_featureList;

};

#endif // CLAYER_H
