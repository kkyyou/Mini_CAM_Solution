#include "savexmlfile.h"
#include "typedefine.h"
#include "layer.h"
#include "job.h"
#include "feature.h"

#include <QFile>

CSaveXMLFile::CSaveXMLFile()
{

}

void CSaveXMLFile::saveJob(QXmlStreamWriter *writer, QFile *file, CJob *job)
{
    if (!job)
        return;

    // START.
    writer->setDevice(file);
    writer->writeStartDocument();
    writer->writeStartElement(_LAYERS);

    QList<CLayer *> layerList = job->layerList();
    for (auto iterLayer = layerList.cbegin(); iterLayer != layerList.cend(); ++iterLayer)
    {
        CLayer *layer = *iterLayer;
        if (!layer)
            continue;

        // SAVE LAYER.
        saveLayer(writer, layer);
    }

    // END.
    writer->writeEndElement();
    writer->writeEndDocument();
}

void CSaveXMLFile::saveLayer(QXmlStreamWriter *writer, CLayer *layer)
{
    QString layerName = layer->layerName();

    // START LAYER.
    writer->writeStartElement(_LAYER);
    writer->writeAttribute(_LAYER_NAME, layerName);

    QList<CFeature *> featureList = layer->featureList();

    for (auto iterFeature = featureList.cbegin(); iterFeature != featureList.cend(); ++iterFeature)
    {
        CFeature *feature = *iterFeature;
        if (!feature)
            continue;

        // SAVE FEATURE.
        saveFeature(writer, feature);
    }

    // END LAYER.
    writer->writeEndElement();
}

void CSaveXMLFile::saveFeature(QXmlStreamWriter *writer, CFeature *feature)
{
    // START FEATURE.
    writer->writeStartElement(_FEATURE);

    FEATURE_TYPE featureType = feature->type();
    switch (featureType)
    {
    case _FEATURE_PAD:  saveFeaturePad(writer, feature);    break;
    case _FEATURE_LINE: saveFeatureLine(writer, feature);   break;
    default:                                                break;
    }

    // END FEATURE.
    writer->writeEndElement();
}

void CSaveXMLFile::saveFeaturePad(QXmlStreamWriter *writer, CFeature *feature)
{
    SHAPE_TYPE shapeType = feature->shape()->type();
    switch (shapeType)
    {
    case _SHAPE_ROUND:  saveFeaturePadRound(writer, feature);   break;
    case _SHAPE_RECT:   saveFeaturePadRect(writer, feature);    break;
    default:                                                    break;
    }
}

void CSaveXMLFile::saveFeatureLine(QXmlStreamWriter *writer, CFeature *feature)
{
    SHAPE_TYPE shapeType = feature->shape()->type();
    switch (shapeType)
    {
    case _SHAPE_ROUND:  saveFeatureLineRound(writer, feature);   break;
    case _SHAPE_RECT:   saveFeatureLineRect(writer, feature);    break;
    default:                                                     break;
    }
}

void CSaveXMLFile::saveFeaturePadRound(QXmlStreamWriter *writer, CFeature *feature)
{
    QPoint center = feature->getCenterPoint();

    writer->writeAttribute(_FEATURE_TYPE, _PAD);
    writer->writeAttribute(_SHAPE_TYPE, _ROUND);
    writer->writeAttribute(_CENTER_X, QString::number(center.x()));
    writer->writeAttribute(_CENTER_Y, QString::number(center.y()));
    writer->writeAttribute(_DIAMETER, QString::number(feature->shape()->getWidth()));
}

void CSaveXMLFile::saveFeaturePadRect(QXmlStreamWriter *writer, CFeature *feature)
{
    QPoint center = feature->getCenterPoint();

    writer->writeAttribute(_FEATURE_TYPE, _PAD);
    writer->writeAttribute(_SHAPE_TYPE, _RECTANGLE);
    writer->writeAttribute(_CENTER_X, QString::number(center.x()));
    writer->writeAttribute(_CENTER_Y, QString::number(center.y()));
    writer->writeAttribute(_WIDTH, QString::number(feature->shape()->getWidth()));
    writer->writeAttribute(_HEIGHT, QString::number(feature->shape()->getHeight()));
}

void CSaveXMLFile::saveFeatureLineRound(QXmlStreamWriter *writer, CFeature *feature)
{
    QPoint start = feature->getStartPoint();
    QPoint end = feature->getEndPoint();

    writer->writeAttribute(_FEATURE_TYPE, _LINE);
    writer->writeAttribute(_SHAPE_TYPE, _ROUND);
    writer->writeAttribute(_START_X, QString::number(start.x()));
    writer->writeAttribute(_START_Y, QString::number(start.y()));
    writer->writeAttribute(_END_X, QString::number(end.x()));
    writer->writeAttribute(_END_Y, QString::number(end.y()));
    writer->writeAttribute(_DIAMETER, QString::number(feature->shape()->getWidth()));
}

void CSaveXMLFile::saveFeatureLineRect(QXmlStreamWriter *writer, CFeature *feature)
{
    QPoint start = feature->getStartPoint();
    QPoint end = feature->getEndPoint();

    writer->writeAttribute(_FEATURE_TYPE, _LINE);
    writer->writeAttribute(_SHAPE_TYPE, _RECTANGLE);
    writer->writeAttribute(_START_X, QString::number(start.x()));
    writer->writeAttribute(_START_Y, QString::number(start.y()));
    writer->writeAttribute(_END_X, QString::number(end.x()));
    writer->writeAttribute(_END_Y, QString::number(end.y()));
    writer->writeAttribute(_WIDTH, QString::number(feature->shape()->getWidth()));
    writer->writeAttribute(_HEIGHT, QString::number(feature->shape()->getHeight()));
}
