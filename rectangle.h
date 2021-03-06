#ifndef CRECTANGLE_H
#define CRECTANGLE_H

#include "shape.h"

class CRectangle : public CShape
{
public:
    CRectangle(const long &width, const long &height);

    virtual long getWidth();
    virtual long getHeight();
    virtual QString getSizeInfo();
    virtual QString getSizeInfoOnlyWidth();

private:
    long m_width;
    long m_height;
};

#endif // CRECTANGLE_H
