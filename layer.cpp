#include "layer.h"

CLayer::CLayer() :
    m_layerName(""),
    m_isActive(false),
    m_isView(false)
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

bool CLayer::isActive() const
{
    return m_isActive;
}

void CLayer::setIsActive(bool isActive)
{
    m_isActive = isActive;
}

bool CLayer::isView() const
{
    return m_isView;
}

void CLayer::setIsView(bool isView)
{
    m_isView = isView;
}
