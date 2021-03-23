#ifndef CPAD_H
#define CPAD_H

#include <QObject>

#include "feature.h"

class CPad : public CFeature
{
public:
    CPad(const FEATURE_TYPE &featureType, const QPoint &center);

    virtual QPoint getCenterPoint();

private:
    QPoint m_center;
};

#endif // CPAD_H
