#ifndef COPENXMLFILE_H
#define COPENXMLFILE_H

class QXmlStreamReader;
class QFile;
class CJob;
class CLayer;
class COpenXMLFile
{
public:
    COpenXMLFile();

    static void openJob(QXmlStreamReader *reader, CJob *job);
    static void openLayers(QXmlStreamReader *reader, CJob *job);
    static void openLayer(QXmlStreamReader *reader, CJob *job);
    static void openFeature(QXmlStreamReader *reader, CLayer *layer);
    static void openFeaturePad(QXmlStreamReader *reader, CLayer *layer);
    static void openFeatureLine(QXmlStreamReader *reader, CLayer *layer);
    static void openFeaturePadRound(QXmlStreamReader *reader, CLayer *layer);
    static void openFeaturePadRect(QXmlStreamReader *reader, CLayer *layer);
    static void openFeatureLineRound(QXmlStreamReader *reader, CLayer *layer);
    static void openFeatureLineRect(QXmlStreamReader *reader, CLayer *layer);
};

#endif // COPENXMLFILE_H
