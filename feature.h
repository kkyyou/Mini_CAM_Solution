#ifndef CFEATURE_H
#define CFEATURE_H

#include <QObject>
#include <QPainterPath>

#include "typedefine.h"
#include "shape.h"
#include "QPoint"

class CFeature
{
public:
    CFeature(const FEATURE_TYPE &type);
    virtual ~CFeature();

    virtual QPoint getCenterPoint() = 0;
    virtual QPoint getStartPoint() { return QPoint(); }
    virtual QPoint getEndPoint()   { return QPoint(); }
    virtual QString getPointInfo() = 0;
    virtual QString getSizeInfo() = 0;
    virtual void calcArea() = 0;

    FEATURE_TYPE type() const;
    void setType(const FEATURE_TYPE &type);
    QString getFeatureTypeString() const;

    CShape *shape() const;
    void setShape(CShape *shape);
    QString getShapeTypeString() const;

    QPainterPath getAreaPath() const;
    void setAreaPath(const QPainterPath &areaPath);

protected:
    QPainterPath m_areaPath;

private:
    FEATURE_TYPE m_type;
    CShape *m_shape;
};

#endif // CFEATURE_H
