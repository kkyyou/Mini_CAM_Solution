#include "feature.h"

CFeature::CFeature(const FEATURE_TYPE &type) :
    m_type(type)
{

}

CFeature::~CFeature()
{
    delete m_shape;
}

FEATURE_TYPE CFeature::type() const
{
    return m_type;
}

void CFeature::setType(const FEATURE_TYPE &type)
{
    m_type = type;
}

QString CFeature::getFeatureTypeString() const
{
    if      (m_type == _FEATURE_PAD)  return QString("PAD");
    else if (m_type == _FEATURE_LINE) return QString("LINE");

    return QString("");
}

CShape *CFeature::shape() const
{
    return m_shape;
}

void CFeature::setShape(CShape *shape)
{
    m_shape = shape;
}

QString CFeature::getShapeTypeString() const
{
    if      (m_shape->type() == _SHAPE_ROUND)  return QString("ROUND");
    else if (m_shape->type() == _SHAPE_RECT)   return QString("RECT");

    return QString("");
}
