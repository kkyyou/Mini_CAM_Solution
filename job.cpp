#include "job.h"

CJob::CJob()
{

}

QList<CLayer *> CJob::layerList() const
{
    return m_layerList;
}

void CJob::setLayerList(const QList<CLayer *> &layerList)
{
    m_layerList = layerList;
}

void CJob::appendLayer(CLayer *layer)
{
    if (!layer)
        return;

    m_layerList.append(layer);
}
