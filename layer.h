#ifndef CLAYER_H
#define CLAYER_H

#include <QObject>
#include <QList>

class CFeature;
class CLayer
{
public:
    CLayer();

    void appendFeature(CFeature *feature);

    QList<CFeature *> featureList() const;
    void setFeatureList(const QList<CFeature *> &featureList);

private:
    QList<CFeature *> m_featureList;
};

#endif // CLAYER_H
