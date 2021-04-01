#include "addfeaturecommand.h"

#include "layer.h"
#include "feature.h"

CAddFeatureCommand::CAddFeatureCommand(CLayer *layer, CFeature *feature) :
    m_layer(layer),
    m_feature(feature)
{

}

void CAddFeatureCommand::undo()
{
    m_layer->removeFeature(m_feature);
}

void CAddFeatureCommand::redo()
{
    m_layer->appendFeature(m_feature);
}
