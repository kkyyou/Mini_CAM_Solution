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
