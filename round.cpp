#include "round.h"

CRound::CRound(const long &radius, const SHAPE_TYPE &type) :
    CShape(type),
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
