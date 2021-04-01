#include "deletefeaturecommand.h"

#include "layer.h"
#include "feature.h"

CDeleteFeatureCommand::CDeleteFeatureCommand(CLayer *layer, CFeature *feature) :
    m_layer(layer),
    m_feature(feature)
{

}

void CDeleteFeatureCommand::undo()
{
    m_layer->appendFeature(m_feature);
}

void CDeleteFeatureCommand::redo()
{
    m_layer->removeFeature(m_feature);
}
