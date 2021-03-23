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


    void goToNext(const QPoint &point);
    void goToPrev(const QPoint &point);
    void run();

    CLayer *activeLayer() const;
    void setActiveLayer(CLayer *activeLayer);

public slots:
    void updateCurMousePositionSlot(long, long);
    void insertCommandValueMapSlot(QString valueName, QVariant value);
    void updateCurCommandSlot(QString command, QString commandShape);

private slots:
    void on_actionAdd_Pad_triggered();

private:
    CJob *m_job;
    CView *m_view;
    CLayer *m_activeLayer;

    int m_commandStep;
    QString m_command;
    QString m_commandShape;
    QMap<QString, QVariant> m_commandVarMap;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
