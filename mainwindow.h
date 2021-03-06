#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QVariant>
#include <QXmlStreamWriter>
#include <QUndoStack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CJob;
class CView;
class CLayer;
class CLayerListModel;
class CFeatureListModel;
class CFeature;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CLayer *activeLayer() const;

    int commandStep() const;
    void setCommandStep(const int &commandStep);

    QMap<QString, QVariant> commandVarMap() const;
    void setCommandVarMap(const QMap<QString, QVariant> &commandVarMap);

    QString command() const;
    void setCommand(const QString &command);

    QString commandShape() const;
    void setCommandShape(const QString &commandShape);

    CJob *job() const;
    void setJob(CJob *job);

    QList<CFeature *> getSelectedFeatures() const;
    void setSelectedFeatures(const QList<CFeature *> &selectedFeatures);

    CLayerListModel *getLayerListModel();
    CFeatureListModel *getFeatureListModel();

    // Command Run.
    void goToNext(const QPoint &point);
    void goToPrev(const QPoint &point);
    void run();
    void initCommandStep();
    void updateCurrentStepUI();

    // Save.
    void saveFile();
    void saveLayerToXML(QXmlStreamWriter *xmlWriter, CLayer *layer);

    // Command.
    void addFeatureCommand(CLayer *layer, CFeature *feature);
    void deleteFeatureCommand(CLayer *layer, CFeature *feature);

    void updatedFeatures();

signals:
    void changedActiveLayerSignal(CLayer *activeLayer);
    void changedJob(CJob *job);

public slots:
    void updateCurMousePositionSlot(long, long);
    void insertCommandValueMapSlot(QString valueName, QVariant value);
    void updateCurCommandSlot(QString command, QString commandShape);
    void addLayerSlot(QString layerName);
    void setActiveLayer(CLayer *activeLayer);

private:
    void resetJob();

private slots:
    void on_actionAdd_Pad_triggered();

    void on_actionAdd_Layer_triggered();

    void on_actionAdd_Line_triggered();

    void on_actionSave_File_triggered();

    void on_actionOpen_File_triggered();

    void on_actionPoint_Select_triggered();

    void on_actionDelete_Feature_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionRectangle_Select_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionNew_File_triggered();

private:
    CJob   *m_job;
    CView  *m_view;
    CLayer *m_activeLayer;

    int m_commandStep;                        // ?????? ???????????? ??????.
    QString m_command;                        // ?????? ????????? ??????(Add Pad, Add Line).
    QString m_commandShape;                   // ?????? ???????????? ??????(Round, Rect).
    QMap<QString, QVariant> m_commandVarMap;  // ??? ??????????????? ????????? ?????????????????? ???????????? ???.

    QList<CFeature *> m_selectedFeatures;

    QUndoStack m_undoStack;                   // Undo Redo ??????.
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
