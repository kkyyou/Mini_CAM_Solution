#ifndef CDELETEFEATURECOMMAND_H
#define CDELETEFEATURECOMMAND_H

#include <QUndoCommand>

class CFeature;
class CLayer;
class CDeleteFeatureCommand : public QUndoCommand
{
public:
    CDeleteFeatureCommand(CLayer *layer, CFeature *feature);

    void undo();
    void redo();

private:
    CLayer *m_layer;
    CFeature *m_feature;
};

#endif // CDELETEFEATURECOMMAND_H
