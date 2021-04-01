#ifndef CADDFEATURECOMMAND_H
#define CADDFEATURECOMMAND_H

#include <QUndoCommand>

class CFeature;
class CLayer;
class CAddFeatureCommand : public QUndoCommand
{
public:
    CAddFeatureCommand(CLayer *layer, CFeature *feature);

    void undo();
    void redo();

private:
    CLayer *m_layer;
    CFeature *m_feature;
};

#endif // CADDFEATURECOMMAND_H
