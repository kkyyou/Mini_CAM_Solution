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
