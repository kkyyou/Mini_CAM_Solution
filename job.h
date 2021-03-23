#ifndef CJOB_H
#define CJOB_H

#include <QObject>
#include <QList>

class CLayer;
class CJob
{
public:
    CJob();

private:
    QList<CLayer *> m_layerList;
};

#endif // CJOB_H
