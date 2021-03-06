#include "job.h"
#include "layer.h"

CJob::CJob()
{
    CLayer *l1 = NULL, *l2 = NULL, *l3 = NULL;
    m_layerAndColor.enqueue(qMakePair(l1, QColor(Qt::red)));
    m_layerAndColor.enqueue(qMakePair(l2, QColor(Qt::green)));
    m_layerAndColor.enqueue(qMakePair(l3, QColor(Qt::blue)));
}

CJob::~CJob()
{
    // Job이 Delete되면 관리하던 Layer도 Delete.
    qDeleteAll(m_layerList);
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

bool CJob::existActiveLayer() const
{
    for (auto iter = m_layerList.cbegin(); iter != m_layerList.cend(); ++iter)
    {
        CLayer *layer = *iter;
        if (!layer)
            continue;

        if (!layer->isActive())
            continue;

        return true;
    }

    return false;
}

QList<CLayer *> CJob::getViewLayerList() const
{
    QList<CLayer *> viewLayerList;
    for (auto iter = m_layerList.cbegin(); iter != m_layerList.cend(); ++iter)
    {
        CLayer *layer = *iter;
        if (!layer)
            continue;

        if (!layer->isView())
            continue;

        viewLayerList.append(layer);
    }

    return viewLayerList;
}

CLayer *CJob::unActiveLayer()
{
    for (auto iter = m_layerList.cbegin(); iter != m_layerList.cend(); ++iter)
    {
        CLayer *layer = *iter;
        if (!layer)
            continue;

        if (!layer->isActive())
            continue;

        layer->setIsActive(false);
        return layer;
    }

    return NULL;
}

QQueue<QPair<CLayer *, QColor> > *CJob::getLayerAndColorQueue()
{
    return &m_layerAndColor;
}

bool CJob::isUsingAllLayerColor() const
{
    for (auto iter = m_layerAndColor.cbegin(); iter != m_layerAndColor.cend(); ++iter)
    {
        QPair<CLayer *, QColor> layerColor = *iter;
        CLayer *layer = layerColor.first;
        if (!layer)
            return false;
    }

    return true;
}
