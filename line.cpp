#include "line.h"

#include <QLine>
#include <QtMath>

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

void CLine::calcArea()
{
    QPainterPath path;

    SHAPE_TYPE shapeType = shape()->type();
    if (shapeType == _SHAPE_ROUND)
    {
        long radius = shape()->getRadius() / 2;
        double y = (double)m_end.y() - (double)m_start.y();
        double x = (double)m_end.x() - (double)m_start.x();
        double radian = qAtan2(y, x);

        QPoint startLeft;
        double slx = (double)m_start.x() + ((double)radius * -qSin(radian));
        double sly = (double)m_start.y() + ((double)radius * qCos(radian));
        startLeft.setX(slx + ((slx) < 0 ? -0.5 : 0.5));
        startLeft.setY(sly + ((sly) < 0 ? -0.5 : 0.5));

        QPoint startRight;
        double srx = (double)m_start.x() + ((double)radius * qSin(radian));
        double sry = (double)m_start.y() + ((double)radius * -qCos(radian));
        startRight.setX(srx + ((srx) < 0 ? -0.5 : 0.5));
        startRight.setY(sry + ((sry) < 0 ? -0.5 : 0.5));

        QPoint endLeft;
        double elx = (double)m_end.x() + ((double)radius * -qSin(radian));
        double ely = (double)m_end.y() + ((double)radius * qCos(radian));
        endLeft.setX(elx + ((elx) < 0 ? -0.5 : 0.5));
        endLeft.setY(ely + ((ely) < 0 ? -0.5 : 0.5));

        QPoint endRight;
        double erx = (double)m_end.x() + ((double)radius * qSin(radian));
        double ery = (double)m_end.y() + ((double)radius * -qCos(radian));
        endRight.setX(erx + ((erx) < 0 ? -0.5 : 0.5));
        endRight.setY(ery + ((ery) < 0 ? -0.5 : 0.5));

        path.moveTo(startLeft);
        path.lineTo(endLeft);
        path.moveTo(endRight);
        path.lineTo(startRight);
    }
    else if (shapeType == _SHAPE_RECT)
    {
        long width = shape()->getWidth();
        long height = shape()->getHeight();
    }

    m_areaPath = path;
}

