#ifndef CSAVEXMLFILE_H
#define CSAVEXMLFILE_H

#include <QXmlStreamWriter>

class CJob;
class QFile;
class CLayer;
class CFeature;
class CSaveXMLFile
{
public:
    CSaveXMLFile();

    static void saveJob(QXmlStreamWriter *writer, QFile *file, CJob *job);
    static void saveLayer(QXmlStreamWriter *writer, CLayer *layer);
    static void saveFeature(QXmlStreamWriter *writer, CFeature *feature);
    static void saveFeaturePad(QXmlStreamWriter *writer, CFeature *feature);
    static void saveFeatureLine(QXmlStreamWriter *writer, CFeature *feature);
    static void saveFeaturePadRound(QXmlStreamWriter *writer, CFeature *feature);
    static void saveFeaturePadRect(QXmlStreamWriter *writer, CFeature *feature);
    static void saveFeatureLineRound(QXmlStreamWriter *writer, CFeature *feature);
    static void saveFeatureLineRect(QXmlStreamWriter *writer, CFeature *feature);
};

#endif // CSAVEXMLFILE_H
