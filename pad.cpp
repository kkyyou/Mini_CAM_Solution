#include "pad.h"

#include <QPainterPath>

CPad::CPad(const QPoint &center) :
    CFeature(_FEATURE_PAD),
    m_center(center)
{

}

QPoint CPad::getCenterPoint()
{
    return m_center;
}

QString CPad::getPointInfo()
{
    QPoint center = getCenterPoint();
    double dx = (double) center.x() / _MILLION;
    double dy = (double) center.y() / _MILLION;

    QString strX = QString::number(dx, 'f', 3);
    QString strY = QString::number(dy, 'f', 3);

    return "x = " + strX + ", " + "y = " + strY;
}

QString CPad::getSizeInfo()
{
    return shape()->getSizeInfo();
}

void CPad::calcArea()
{
    QPainterPath path;

    SHAPE_TYPE shapeType = shape()->type();
    if (shapeType == _SHAPE_ROUND)
    {
        long radius = shape()->getRadius();
        path.addEllipse(m_center, radius, radius);
    }
    else if (shapeType == _SHAPE_RECT)
    {
        long width = shape()->getWidth();
        long height = shape()->getHeight();

        long left = m_center.x() - (width / 2);
        long right = m_center.x() + (width / 2);
        long bottom = m_center.y() - (height / 2);
        long top = m_center.y() + (height / 2);

        path.moveTo(QPoint(left, bottom));
        path.lineTo(QPoint(left, top));
        path.lineTo(QPoint(right, top));
        path.lineTo(QPoint(right, bottom));
        path.lineTo(QPoint(left, bottom));
    }

    m_areaPath = path;
}
