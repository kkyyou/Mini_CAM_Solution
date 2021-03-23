#include "rectangle.h"

CRectangle::CRectangle(const long &width, const long &height, const SHAPE_TYPE &type) :
    CShape(type),
    m_width(width),
    m_height(height)
{

}

long CRectangle::getWidth()
{
    return m_width;
}

long CRectangle::getHeight()
{
    return m_height;
}
