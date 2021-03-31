#ifndef CJOB_H
#define CJOB_H

#include <QObject>
#include <QList>
#include <QQueue>

class CLayer;
class CJob : public QObject
{
public:
    CJob();
    ~CJob();

    QList<CLayer *> layerList() const;
    void setLayerList(const QList<CLayer *> &layerList);

    void appendLayer(CLayer *layer);

    bool existActiveLayer() const;
    QList<CLayer *> getViewLayerList() const;

    CLayer *unActiveLayer();

    QQueue<QPair<CLayer *, QColor>> *getLayerAndColorQueue();
    bool isUsingAllLayerColor() const;

private:
    QList<CLayer *> m_layerList;
    QQueue<QPair<CLayer *, QColor>> m_layerAndColor;
};

#endif // CJOB_H
