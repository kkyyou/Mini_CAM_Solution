#include "line.h"

CLine::CLine(const QPoint &start, const QPoint &end) :
    CFeature(_FEATURE_LINE),
    m_start(start),
    m_end(end)
{

}

QPoint CLine::getCenterPoint()
{
    return (m_start + m_end) / 2;
}

QPoint CLine::getStartPoint()
{
    return m_start;
}

QPoint CLine::getEndPoint()
{
    return m_end;
}
