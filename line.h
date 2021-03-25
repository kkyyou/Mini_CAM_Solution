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

private:
    QPoint m_start;
    QPoint m_end;
};

#endif // CLINE_H
