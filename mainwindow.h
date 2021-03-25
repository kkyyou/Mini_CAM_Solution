#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QVariant>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CJob;
class CView;
class CLayer;

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

    // Command Run.
    void goToNext(const QPoint &point);
    void goToPrev(const QPoint &point);
    void run();
    void initCommandStep();
    void updateCurrentStepUI();

public slots:
    void updateCurMousePositionSlot(long, long);
    void insertCommandValueMapSlot(QString valueName, QVariant value);
    void updateCurCommandSlot(QString command, QString commandShape);
    void addLayerSlot(QString layerName);
    void setActiveLayer(CLayer *activeLayer);

private slots:
    void on_actionAdd_Pad_triggered();

    void on_actionAdd_Layer_triggered();

    void on_actionAdd_Line_triggered();

private:
    CJob   *m_job;
    CView  *m_view;
    CLayer *m_activeLayer;

    int m_commandStep;                        // 현재 커맨드의 순서.
    QString m_command;                        // 현재 커맨드 이름(Add Pad, Add Line).
    QString m_commandShape;                   // 현재 커맨드의 모양(Round, Rect).
    QMap<QString, QVariant> m_commandVarMap;  // 각 커맨드에서 필요한 파라미터들을 저장하는 맵.
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
