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
        long radius = shape()->getRadius();

        // 기울기 구하기.
        double radian = getRadian(m_start, m_end);
        double degree = radian / _PI * 180.0;
        double othogonalRightDegree = degree - 90.0;

        // 기울기에 직교하며 Start , End 점에서 반지름 만큼 떨어진 두 점 구하기.
        QPair<QPoint, QPoint> startLeftRight = getOthogonalLeftRightPoint(m_start, radian, radius);
        QPair<QPoint, QPoint> endLeftRight = getOthogonalLeftRightPoint(m_end, radian, radius);

        QPoint startLeft = startLeftRight.first;
        QPoint startRight = startLeftRight.second;

        QPoint endLeft = endLeftRight.first;
        QPoint endRight = endLeftRight.second;

        //! 원을 내접하고있는 사각형(Rect) 구하기.
        // End Point쪽 원을 내접하는 사각형 구하기.
        QPoint endRectLeftTop(m_end.x() - radius, m_end.y() + radius);
        QRect tmpRect1(endRectLeftTop.x(), endRectLeftTop.y(), radius * 2, -radius * 2);

        // Start Point 쪽 원을 내접하는 사각형 구하기.
        QPoint startRectLeftTop(m_start.x() - radius, m_start.y() + radius);
        QRect tmpRect2(startRectLeftTop.x(), startRectLeftTop.y(), radius * 2, -radius * 2);

        // Path 그리기.
        path.moveTo(startLeft);
        path.lineTo(endLeft);
        path.moveTo(endRight);
        path.arcTo(tmpRect1, othogonalRightDegree, 180);   // 현재 라인의 직교하는 각도를 Start로 180도 만큼 호 그리기.
        path.moveTo(endRight);
        path.lineTo(startRight);
        path.arcTo(tmpRect2, othogonalRightDegree, -180);  // 현재 라인의 직교하는 각도를 Start로 -180도 만큼 호 그리기.
    }
    else if (shapeType == _SHAPE_RECT)
    {
        long widthHalf = shape()->getWidth() / 2;

        // 기울기 구하기.
        double radian = getRadian(m_start, m_end);

        // 기울기에 직교하며 Start , End 점에서 Width Half 만큼 떨어진 두 점 구하기.
        QPair<QPoint, QPoint> startLeftRight = getOthogonalLeftRightPoint(m_start, radian, widthHalf);
        QPair<QPoint, QPoint> endLeftRight = getOthogonalLeftRightPoint(m_end, radian, widthHalf);

        // 외곽 사각형 아웃라인을 잡기위해 위에서 구한 점들을 widthHalf만큼 라인선상의 점으로 이동한다.
        QPoint startLeft = startLeftRight.first;
        startLeft.setX(startLeft.x() + (-widthHalf * qCos(radian)));
        startLeft.setY(startLeft.y() + (-widthHalf * qSin(radian)));

        QPoint startRight = startLeftRight.second;
        startRight.setX(startRight.x() + (-widthHalf * qCos(radian)));
        startRight.setY(startRight.y() + (-widthHalf * qSin(radian)));

        QPoint endLeft = endLeftRight.first;
        endLeft.setX(endLeft.x() + (widthHalf * qCos(radian)));
        endLeft.setY(endLeft.y() + (widthHalf * qSin(radian)));

        QPoint endRight = endLeftRight.second;
        endRight.setX(endRight.x() + (widthHalf * qCos(radian)));
        endRight.setY(endRight.y() + (widthHalf * qSin(radian)));

        // Path 그리기.
        path.moveTo(startLeft);
        path.lineTo(endLeft);
        path.lineTo(endRight);
        path.lineTo(startRight);
        path.lineTo(startLeft);
    }

    m_areaPath = path;
}

double CLine::getRadian(QPoint start, QPoint end)
{
    double y = (double)end.y() - (double)start.y();
    double x = (double)end.x() - (double)start.x();

    double radian = qAtan2(y, x);

    return radian;
}

QPair<QPoint, QPoint> CLine::getOthogonalLeftRightPoint(QPoint anchor, double radian, long distance)
{
    QPoint anchorLeft, anchorRight;

    double sinValue = qSin(radian);
    double cosValue = qCos(radian);

    // cos(90+a) = -sin(a)
    // cos(270+a) = sin(a)
    // sin(90+a) = cos(a)
    // sin(270+a) = -cos(a)

    // LEFT.
    double lx = (double)anchor.x() + (double)distance * -sinValue;
    double ly = (double)anchor.y() + (double)distance * cosValue;
    anchorLeft.setX(lx + ((lx) < 0 ? -0.5 : 0.5));
    anchorLeft.setY(ly + ((ly) < 0 ? -0.5 : 0.5));

    // RIGHT.
    double rx = (double)anchor.x() + (double)distance * sinValue;
    double ry = (double)anchor.y() + (double)distance * -cosValue;
    anchorRight.setX(rx + ((rx) < 0 ? -0.5 : 0.5));
    anchorRight.setY(ry + ((ry) < 0 ? -0.5 : 0.5));

    return qMakePair(anchorLeft, anchorRight);
}

