#include "layer.h"

CLayer::CLayer() :
    m_layerName(""),
    m_isActive(false),
    m_isView(false),
    m_featureColor(Qt::white)
{

}

CLayer::~CLayer()
{
    // 레이어가 Delete 되면 관리하던 Feature도 Delete.
    qDeleteAll(m_featureList);
}

void CLayer::appendFeature(CFeature *feature)
{
    m_featureList.append(feature);
    emit updatedFeatureListSignal(this);
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
    initFeatureColor();
}

QColor CLayer::featureColor() const
{
    return m_featureColor;
}

void CLayer::setFeatureColor(const QColor &featureColor)
{
    m_featureColor = featureColor;
}

void CLayer::initFeatureColor()
{
    if (this->isView())
        return;

    this->setFeatureColor(Qt::white);
}
