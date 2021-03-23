#ifndef CFEATURE_H
#define CFEATURE_H

#include <QObject>

#include "typedefine.h"
#include "shape.h"
#include "QPoint"

class CFeature
{
public:
    CFeature(const FEATURE_TYPE &type);

    virtual QPoint getCenterPoint() = 0;
    virtual QPoint getStartPoint() { return QPoint(); }
    virtual QPoint getEndPoint()   { return QPoint(); }

    FEATURE_TYPE type() const;
    void setType(const FEATURE_TYPE &type);

    CShape *shape() const;
    void setShape(CShape *shape);


private:
    FEATURE_TYPE m_type;
    CShape *m_shape;
};

#endif // CFEATURE_H
