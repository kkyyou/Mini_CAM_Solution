#include "round.h"

CRound::CRound(const long &diameter) :
    CShape(_SHAPE_ROUND),
    m_diameter(diameter)
{

}

long CRound::getRadius()
{
    return m_diameter / 2;
}

long CRound::getWidth()
{
    return m_diameter;
}

long CRound::getHeight()
{
    return m_diameter;
}

QString CRound::getSizeInfo()
{
    QString diameter = QString::number(m_diameter / _MILLION);
    return QString("Diameter = " + diameter);
}
