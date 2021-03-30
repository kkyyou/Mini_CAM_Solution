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

QString CLine::getPointInfo()
{
    QPoint start = getStartPoint();
    QPoint end = getEndPoint();

    double dsx = (double) start.x() / _MILLION;
    double dsy = (double) start.y() / _MILLION;
    double dex = (double) end.x() / _MILLION;
    double dey = (double) end.y() / _MILLION;

    QString strSX = QString::number(dsx, 'f', 3);
    QString strSY = QString::number(dsy, 'f', 3);
    QString strEX = QString::number(dex, 'f', 3);
    QString strEY = QString::number(dey, 'f', 3);

    return "s = (" + strSX + ", " + strSY + ")"
            + " e = (" + strEX + ", " + strEY + ")";
}

QString CLine::getSizeInfo()
{
    CShape *s = shape();
    SHAPE_TYPE st = s->type();

    if (st == _SHAPE_ROUND)         return s->getSizeInfo();
    else if (st == _SHAPE_RECT)     return s->getSizeInfoOnlyWidth();

    return "";
}
