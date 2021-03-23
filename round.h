#ifndef CROUND_H
#define CROUND_H

#include "shape.h"

class CRound : public CShape
{
public:
    CRound(const long &radius, const SHAPE_TYPE &type);

    virtual long getRadius();
    virtual long getWidth();
    virtual long getHeight();

private:
    long m_radius;
};

#endif // CROUND_H