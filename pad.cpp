#include "pad.h"

CPad::CPad(const QPoint &center) :
    CFeature(_FEATURE_PAD),
    m_center(center)
{

}

QPoint CPad::getCenterPoint()
{
    return m_center;
}
