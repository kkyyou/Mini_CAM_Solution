#include "rectangle.h"

CRectangle::CRectangle(const long &width, const long &height) :
    CShape(_SHAPE_RECT),
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
