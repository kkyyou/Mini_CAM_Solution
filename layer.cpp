#include "layer.h"

CLayer::CLayer()
{

}

void CLayer::appendFeature(CFeature *feature)
{
    m_featureList.append(feature);
}

QList<CFeature *> CLayer::featureList() const
{
    return m_featureList;
}

void CLayer::setFeatureList(const QList<CFeature *> &featureList)
{
    m_featureList = featureList;
}

QString CLayer::layerName() const
{
    return m_layerName;
}

void CLayer::setLayerName(const QString &layerName)
{
    m_layerName = layerName;
}
