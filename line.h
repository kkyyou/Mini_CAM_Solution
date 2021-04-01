#ifndef CLINE_H
#define CLINE_H

#include <QObject>

#include "feature.h"

#include <QPoint>

class CLine : public CFeature
{
public:
    CLine(const QPoint &start, const QPoint &end);

    virtual QPoint getCenterPoint();
    virtual QPoint getStartPoint();
    virtual QPoint getEndPoint();
    virtual QString getPointInfo();
    virtual QString getSizeInfo();
    virtual void calcArea();

    double getRadian(QPoint start, QPoint end);
    QPair<QPoint, QPoint> getOthogonalLeftRightPoint(QPoint anchor, double radian, long distance);

private:
    QPoint m_start;
    QPoint m_end;
};

#endif // CLINE_H
