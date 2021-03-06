#ifndef CPAD_H
#define CPAD_H

#include <QObject>

#include "feature.h"

class CPad : public CFeature
{
public:
    CPad(const QPoint &center);

    virtual QPoint getCenterPoint();
    virtual QString getPointInfo();
    virtual QString getSizeInfo();
    virtual void calcArea();

private:
    QPoint m_center;
};

#endif // CPAD_H
