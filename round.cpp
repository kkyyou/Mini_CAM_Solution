#include "round.h"

CRound::CRound(const long &radius) :
    CShape(_SHAPE_ROUND),
    m_radius(radius)
{

}

long CRound::getRadius()
{
    return m_radius;
}

long CRound::getWidth()
{
    return m_radius * 2;
}

long CRound::getHeight()
{
    return m_radius * 2;
}
