#ifndef CSHAPE_H
#define CSHAPE_H

#include "typedefine.h"
#include <QString>

class CShape
{
public:
    CShape(const SHAPE_TYPE &type);
    virtual ~CShape();

    virtual long getRadius()    { return 0; }
    virtual long getWidth() = 0;
    virtual long getHeight() = 0;
    virtual QString getSizeInfo() = 0;
    virtual QString getSizeInfoOnlyWidth() { return ""; }

    SHAPE_TYPE type() const;
    void setType(const SHAPE_TYPE &type);

private:
    SHAPE_TYPE m_type;
};

#endif // CSHAPE_H
