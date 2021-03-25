#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "job.h"
#include "view.h"
#include "typedefine.h"
#include "addpaddialog.h"
#include "feature.h"
#include "pad.h"
#include "shape.h"
#include "round.h"
#include "rectangle.h"
#include "layer.h"
#include "layerlistmodel.h"
#include "addlayerdialog.h"
#include "radiobuttondelegate.h"
#include "checkboxdelegate.h"
#include "line.h"
#include "addlinedialog.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_activeLayer(NULL)
{
    ui->setupUi(this);

    m_job = new CJob();
    m_view = new CView(this);

    // 중앙에 Widget(드로우 영역) 배치.
    setCentralWidget(m_view);

    // Model.
    CLayerListModel *layerListModel = new CLayerListModel(m_job, this);
    ui->layer_list_view->setModel(layerListModel);
    ui->layer_list_view->setColumnWidth(CLayerListModel::_COLUMN_ACTIVE, 60);
    ui->layer_list_view->setColumnWidth(CLayerListModel::_COLUMN_VIEW, 60);
    ui->layer_list_view->horizontalHeader()->setStretchLastSection(true);

    // Delegate.
    CCheckBoxDelegate *activeLayerCheckBoxDelegate = new CCheckBoxDelegate(this);
    ui->layer_list_view->setItemDelegateForColumn(CLayerListModel::_COLUMN_ACTIVE, activeLayerCheckBoxDelegate);

    CCheckBoxDelegate *viewLayerCheckBoxDelegate = new CCheckBoxDelegate(this);
    ui->layer_list_view->setItemDelegateForColumn(CLayerListModel::_COLUMN_VIEW, viewLayerCheckBoxDelegate);

    // Connect.
    QObject::connect(layerListModel, SIGNAL(changeActiveLayer(CLayer *)), this, SLOT(setActiveLayer(CLayer *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::goToNext(const QPoint &point)
{
    // 현재 커맨드 체크.
    if (m_command.compare(_ADD_PAD) == 0)
    {
        // 현재 커맨드의 Step 체크.
        switch (m_commandStep)
        {
        case 0:
        {
            m_commandVarMap.insert(_CENTER_PT, point);
            run();
        } break;
        }
    }
    else if (m_command.compare(_ADD_LINE) == 0)
    {
        switch (m_commandStep)
        {
        case 0:
        {
            m_commandVarMap.insert(_START_PT, point);
            m_commandStep++;
            updateCurrentStepUI();
        } break;
        case 1:
        {
            m_commandVarMap.insert(_END_PT, point);
            initCommandStep();
            updateCurrentStepUI();
            run();
        } break;
        }
    }
}

void MainWindow::goToPrev(const QPoint &point)
{

}

void MainWindow::run()
{
    if (!m_activeLayer)
        return;

    // 커맨드에 따른 실행.
    if (m_command.compare(_ADD_PAD) == 0)
    {
        QPoint pos = m_commandVarMap.value(_CENTER_PT).toPoint();

        CFeature *feature = new CPad(pos);

        if (m_commandShape.compare(_ROUND) == 0)
        {
            qlonglong radius = m_commandVarMap.value(_RADIUS).toLongLong();
            CShape *shape = new CRound(radius);

            feature->setShape(shape);
            m_activeLayer->appendFeature(feature);
        }
        else if (m_commandShape.compare(_RECTANGLE) == 0)
        {
            qlonglong width = m_commandVarMap.value(_WIDTH).toLongLong();
            qlonglong height = m_commandVarMap.value(_HEIGHT).toLongLong();
            CShape *shape = new CRectangle(width, height);

            feature->setShape(shape);
            m_activeLayer->appendFeature(feature);
        }
    }
    else if (m_command.compare(_ADD_LINE) == 0)
    {
        QPoint start = m_commandVarMap.value(_START_PT).toPoint();
        QPoint end = m_commandVarMap.value(_END_PT).toPoint();

        CFeature *feature = new CLine(start, end);

        if (m_commandShape.compare(_ROUND) == 0)
        {
            qlonglong radius = m_commandVarMap.value(_RADIUS).toLongLong();
            CShape *shape = new CRound(radius);

            feature->setShape(shape);
            m_activeLayer->appendFeature(feature);
        }
        else if (m_commandShape.compare(_RECTANGLE) == 0)
        {
            qlonglong width = m_commandVarMap.value(_WIDTH).toLongLong();
            qlonglong height = m_commandVarMap.value(_HEIGHT).toLongLong();
            CShape *shape = new CRectangle(width, height);

            feature->setShape(shape);
            m_activeLayer->appendFeature(feature);
        }
    }
}

void MainWindow::initCommandStep()
{
    m_commandStep = 0;
}

void MainWindow::updateCurrentStepUI()
{
    ui->currentStep->setText("Step : " + QString::number(m_commandStep));
}

void MainWindow::updateCurMousePositionSlot(long x, long y)
{
    double dx, dy;
    dx = (double)x / _MILLION;
    dy = (double)y / _MILLION;

    QString xStr = QString::number(dx);
    QString yStr = QString::number(dy);

    // 마우스 이동 시 좌표 업데이트.
    ui->curMousePosition_label->setText("x = "  + xStr.sprintf("%6f", dx) + " , " "y = " + yStr.sprintf("%6f", dy));
}

void MainWindow::insertCommandValueMapSlot(QString valueName, QVariant value)
{
    m_commandVarMap.insert(valueName, value);
}

void MainWindow::updateCurCommandSlot(QString command, QString commandShape)
{
    m_command = command;
    m_commandShape = commandShape;

    ui->currentCommand->setText("Command : " + command + " (" + commandShape + ")");
}

void MainWindow::addLayerSlot(QString layerName)
{
    CLayerListModel *layerListModel = qobject_cast<CLayerListModel *>(ui->layer_list_view->model());
    if (!layerListModel)
        return;

    // 새로운 레이어 생성.
    CLayer *newLayer = new CLayer();
    newLayer->setLayerName(layerName);
    m_job->appendLayer(newLayer);

    // 레이어 리스트 모델의 기존 리스트 가져와서 새로운 레이어 추가 후 다시 셋팅.
    QList<CLayer *> layerList = layerListModel->layerList();
    if (layerList.isEmpty())
    {
        // 처음 생성하는 레이어면 Active , View.
        newLayer->setIsActive(true);
        newLayer->setIsView(true);
        m_activeLayer = newLayer;
    }

    layerList.append(newLayer);
    layerListModel->setLayerList(layerList);
}


void MainWindow::on_actionAdd_Pad_triggered()
{
    if (!m_activeLayer)
    {
        QMessageBox::warning(this, tr("Empty Active Layer"), "Select Active Layer.", QMessageBox::Yes);
        return;
    }

    CAddPadDialog *addPadDlg = new CAddPadDialog(this);
    addPadDlg->setWindowTitle("Add Pad");
    addPadDlg->show();

    // Connect.
    QObject::connect(addPadDlg, SIGNAL(insertCommandValueSignal(QString, QVariant)), this, SLOT(insertCommandValueMapSlot(QString, QVariant)));
    QObject::connect(addPadDlg, SIGNAL(updateCurrentCommandSignal(QString, QString)), this, SLOT(updateCurCommandSlot(QString, QString)));
}

CLayer *MainWindow::activeLayer() const
{
    return m_activeLayer;
}

void MainWindow::setActiveLayer(CLayer *activeLayer)
{
    m_activeLayer = activeLayer;
}

void MainWindow::on_actionAdd_Layer_triggered()
{
    CAddLayerDialog *addLayerDlg = new CAddLayerDialog(this);
    addLayerDlg->show();

    // Add Layer 시그널/슬롯 연결.
    QObject::connect(addLayerDlg, SIGNAL(addLayerSignal(QString)), this, SLOT(addLayerSlot(QString)));
}

void MainWindow::on_actionAdd_Line_triggered()
{
    if (!m_activeLayer)
    {
        QMessageBox::warning(this, tr("Empty Active Layer"), "Select Active Layer.", QMessageBox::Yes);
        return;
    }

    CAddLineDialog *addLineDlg = new CAddLineDialog(this);
    addLineDlg->setWindowTitle("Add Line");
    addLineDlg->show();

    // Connect.
    QObject::connect(addLineDlg, SIGNAL(insertCommandValueSignal(QString, QVariant)), this, SLOT(insertCommandValueMapSlot(QString, QVariant)));
    QObject::connect(addLineDlg, SIGNAL(updateCurrentCommandSignal(QString, QString)), this, SLOT(updateCurCommandSlot(QString, QString)));
}

QString MainWindow::commandShape() const
{
    return m_commandShape;
}

void MainWindow::setCommandShape(const QString &commandShape)
{
    m_commandShape = commandShape;
}

QString MainWindow::command() const
{
    return m_command;
}

void MainWindow::setCommand(const QString &command)
{
    m_command = command;
}

QMap<QString, QVariant> MainWindow::commandVarMap() const
{
    return m_commandVarMap;
}

void MainWindow::setCommandVarMap(const QMap<QString, QVariant> &commandVarMap)
{
    m_commandVarMap = commandVarMap;
}

int MainWindow::commandStep() const
{
    return m_commandStep;
}

void MainWindow::setCommandStep(const int &commandStep)
{
    m_commandStep = commandStep;
}
