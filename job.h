#ifndef CJOB_H
#define CJOB_H

#include <QObject>
#include <QList>

class CLayer;
class CJob
{
public:
    CJob();

    QList<CLayer *> layerList() const;
    void setLayerList(const QList<CLayer *> &layerList);

    void appendLayer(CLayer *layer);
private:
    QList<CLayer *> m_layerList;
};

#endif // CJOB_H
