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

QString CRectangle::getSizeInfo()
{
    QString width = QString::number(m_width / _MILLION);
    QString height = QString::number(m_height / _MILLION);
    return QString("Width = " + width) + " " + QString("Height = " + height);
}

QString CRectangle::getSizeInfoOnlyWidth()
{
    QString width = QString::number(m_width / _MILLION);
    return QString("Width = " + width);
}
