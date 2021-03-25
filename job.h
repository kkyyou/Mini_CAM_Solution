#ifndef CJOB_H
#define CJOB_H

#include <QObject>
#include <QList>

class CLayer;
class CJob
{
public:
    CJob();
    ~CJob();

    QList<CLayer *> layerList() const;
    void setLayerList(const QList<CLayer *> &layerList);

    void appendLayer(CLayer *layer);

    bool existActiveLayer() const;
    QList<CLayer *> getViewLayerList() const;

    void unActiveLayer();
private:
    QList<CLayer *> m_layerList;
};

#endif // CJOB_H
