#ifndef CLAYER_H
#define CLAYER_H

#include <QObject>
#include <QList>
#include <QColor>

class CFeature;
class CLayer : public QObject
{
    Q_OBJECT
public:
    CLayer();
    ~CLayer();

    void appendFeature(CFeature *feature);
    void removeFeature(CFeature *feature);

    QList<CFeature *> featureList() const;
    void setFeatureList(const QList<CFeature *> &featureList);

    QString layerName() const;
    void setLayerName(const QString &layerName);

    bool isActive() const;
    void setIsActive(bool isActive);

    bool isView() const;
    void setIsView(bool isView);

    QColor featureColor() const;
    void setFeatureColor(const QColor &featureColor);

    void initFeatureColor();

signals:
    void updatedFeatureListSignal(CLayer *layer);

private:
    QString m_layerName;
    bool m_isActive;
    bool m_isView;

    QList<CFeature *> m_featureList;
    QColor m_featureColor;

};

#endif // CLAYER_H
