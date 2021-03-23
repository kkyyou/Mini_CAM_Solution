#include "pad.h"

CPad::CPad(const FEATURE_TYPE &featureType, const QPoint &center) :
    CFeature(featureType),
    m_center(center)
{

}

QPoint CPad::getCenterPoint()
{
    return m_center;
}
