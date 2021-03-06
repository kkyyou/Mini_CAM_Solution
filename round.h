#ifndef CROUND_H
#define CROUND_H

#include "shape.h"

class CRound : public CShape
{
public:
    CRound(const long &diameter);

    virtual long getRadius();
    virtual long getWidth();
    virtual long getHeight();
    virtual QString getSizeInfo();

private:
    long m_diameter;
};

#endif // CROUND_H
