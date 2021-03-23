#include "shape.h"

CShape::CShape(const SHAPE_TYPE &type) :
    m_type(type)
{

}

SHAPE_TYPE CShape::type() const
{
    return m_type;
}

void CShape::setType(const SHAPE_TYPE &type)
{
    m_type = type;
}
