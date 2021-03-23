#ifndef CRECTANGLE_H
#define CRECTANGLE_H

#include "shape.h"

class CRectangle : public CShape
{
public:
    CRectangle(const long &width, const long &height, const SHAPE_TYPE &type);

    virtual long getWidth();
    virtual long getHeight();

private:
    long m_width;
    long m_height;
};

#endif // CRECTANGLE_H
