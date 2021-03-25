#ifndef CLAYER_H
#define CLAYER_H

#include <QObject>
#include <QList>

class CFeature;
class CLayer
{
public:
    CLayer();
    ~CLayer();

    void appendFeature(CFeature *feature);

    QList<CFeature *> featureList() const;
    void setFeatureList(const QList<CFeature *> &featureList);

    QString layerName() const;
    void setLayerName(const QString &layerName);

    bool isActive() const;
    void setIsActive(bool isActive);

    bool isView() const;
    void setIsView(bool isView);

private:
    QString m_layerName;
    bool m_isActive;
    bool m_isView;

    QList<CFeature *> m_featureList;

};

#endif // CLAYER_H
