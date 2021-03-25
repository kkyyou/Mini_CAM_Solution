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

CShape *CFeature::shape() const
{
    return m_shape;
}

void CFeature::setShape(CShape *shape)
{
    m_shape = shape;
}
