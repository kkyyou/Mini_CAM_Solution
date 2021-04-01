#include "openxmlfile.h"
#include "job.h"
#include "typedefine.h"
#include "layer.h"
#include "feature.h"
#include "pad.h"
#include "round.h"
#include "rectangle.h"
#include "line.h"

#include <QFile>
#include <QXmlStreamReader>

COpenXMLFile::COpenXMLFile()
{

}

void COpenXMLFile::openJob(QXmlStreamReader *reader, CJob *job)
{
    if (!reader->readNextStartElement())
        return;

    QString elementName = reader->name().toString();
    if (elementName.compare(_LAYERS) == 0)
    {
        openLayers(reader, job);
    }
}

void COpenXMLFile::openLayers(QXmlStreamReader *reader, CJob *job)
{
    while (reader->readNextStartElement())
    {
        QString elementName = reader->name().toString();
        if (elementName.compare(_LAYER) == 0)
        {
            openLayer(reader, job);
        }
    }
}

void COpenXMLFile::openLayer(QXmlStreamReader *reader, CJob *job)
{
    QString layerName = reader->attributes().value(_LAYER_NAME).toString();

    // NEW LAYER.
    CLayer *newLayer = new CLayer();
    newLayer->setLayerName(layerName);
    job->appendLayer(newLayer);

    while (reader->readNextStartElement())
    {
        QString elementName = reader->name().toString();
        if (elementName.compare(_FEATURE) == 0)
        {
            openFeature(reader, newLayer);
            reader->skipCurrentElement();
        }
    }

}

void COpenXMLFile::openFeature(QXmlStreamReader *reader, CLayer *layer)
{
    QString featureType = reader->attributes().value(_FEATURE_TYPE).toString();
    if (featureType.compare(_PAD) == 0)
    {
        openFeaturePad(reader, layer);
    }
    else if (featureType.compare(_LINE) == 0)
    {
        openFeatureLine(reader, layer);
    }
}

void COpenXMLFile::openFeaturePad(QXmlStreamReader *reader, CLayer *layer)
{
    QString shapeType = reader->attributes().value(_SHAPE_TYPE).toString();
    if (shapeType.compare(_ROUND) == 0)
    {
        openFeaturePadRound(reader, layer);
    }
    else if (shapeType.compare(_RECTANGLE) == 0)
    {
        openFeaturePadRect(reader, layer);
    }
}

void COpenXMLFile::openFeatureLine(QXmlStreamReader *reader, CLayer *layer)
{
    QString shapeType = reader->attributes().value(_SHAPE_TYPE).toString();
    if (shapeType.compare(_ROUND) == 0)
    {
        openFeatureLineRound(reader, layer);
    }
    else if (shapeType.compare(_RECTANGLE) == 0)
    {
        openFeatureLineRect(reader, layer);
    }
}

void COpenXMLFile::openFeaturePadRound(QXmlStreamReader *reader, CLayer *layer)
{
    long centerX = reader->attributes().value(_CENTER_X).toLongLong();
    long centerY = reader->attributes().value(_CENTER_Y).toLongLong();
    long diameter = reader->attributes().value(_DIAMETER).toLongLong();

    QPoint center = QPoint(centerX, centerY);

    // NEW FEATURE.
    CFeature *newFeature = new CPad(center);
    CShape *newShape = new CRound(diameter);

    newFeature->setShape(newShape);
    newFeature->calcArea();

    // APPEND FEATURE.
    layer->appendFeature(newFeature);
}

void COpenXMLFile::openFeaturePadRect(QXmlStreamReader *reader, CLayer *layer)
{
    long centerX = reader->attributes().value(_CENTER_X).toLongLong();
    long centerY = reader->attributes().value(_CENTER_Y).toLongLong();
    long width = reader->attributes().value(_WIDTH).toLongLong();
    long height = reader->attributes().value(_HEIGHT).toLongLong();

    QPoint center = QPoint(centerX, centerY);

    // NEW FEATURE.
    CFeature *newFeature = new CPad(center);
    CShape *newShape = new CRectangle(width, height);

    newFeature->setShape(newShape);
    newFeature->calcArea();

    // APPEND FEATURE.
    layer->appendFeature(newFeature);
}

void COpenXMLFile::openFeatureLineRound(QXmlStreamReader *reader, CLayer *layer)
{
    long startX = reader->attributes().value(_START_X).toLongLong();
    long startY = reader->attributes().value(_START_Y).toLongLong();
    long endX = reader->attributes().value(_END_X).toLongLong();
    long endY = reader->attributes().value(_END_Y).toLongLong();
    long diameter = reader->attributes().value(_DIAMETER).toLongLong();

    QPoint start = QPoint(startX, startY);
    QPoint end = QPoint(endX, endY);

    // NEW FEATURE.
    CFeature *newFeature = new CLine(start, end);
    CShape *newShape = new CRound(diameter);

    newFeature->setShape(newShape);
    newFeature->calcArea();

    // APPEND FEATURE.
    layer->appendFeature(newFeature);
}

void COpenXMLFile::openFeatureLineRect(QXmlStreamReader *reader, CLayer *layer)
{
    long startX = reader->attributes().value(_START_X).toLongLong();
    long startY = reader->attributes().value(_START_Y).toLongLong();
    long endX = reader->attributes().value(_END_X).toLongLong();
    long endY = reader->attributes().value(_END_Y).toLongLong();
    long width = reader->attributes().value(_WIDTH).toLongLong();
    long height = reader->attributes().value(_HEIGHT).toLongLong();

    QPoint start = QPoint(startX, startY);
    QPoint end = QPoint(endX, endY);

    // NEW FEATURE.
    CFeature *newFeature = new CLine(start, end);
    CShape *newShape = new CRectangle(width, height);

    newFeature->setShape(newShape);
    newFeature->calcArea();

    // APPEND FEATURE.
    layer->appendFeature(newFeature);
}
