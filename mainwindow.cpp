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

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_job = new CJob();
    m_activeLayer = new CLayer();
    m_view = new CView(this);

    // 중앙에 Widget(드로우 영역) 배치.
    setCentralWidget(m_view);

    // 레이어 모델 셋팅.
    CLayerListModel *layerListModel = new CLayerListModel(this);
    ui->layer_list_view->setModel(layerListModel);
    ui->layer_list_view->setColumnWidth(CLayerListModel::_COLUMN_ACTIVE, 60);
    ui->layer_list_view->setColumnWidth(CLayerListModel::_COLUMN_VIEW, 60);
    ui->layer_list_view->horizontalHeader()->setStretchLastSection(true);
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

    }
}

void MainWindow::goToPrev(const QPoint &point)
{

}

void MainWindow::run()
{
    if (!m_activeLayer)
        return;

    if (m_command.compare(_ADD_PAD) == 0)
    {
        QPoint pos = m_commandVarMap.value(_CENTER_PT).toPoint();

        CFeature *feature = new CPad(_FEATURE_PAD, pos);

        if (m_commandShape.compare(_ROUND) == 0)
        {
            qlonglong radius = m_commandVarMap.value(_RADIUS).toLongLong();

            CShape *shape = new CRound(radius, _SHAPE_ROUND);

            feature->setShape(shape);
            m_activeLayer->appendFeature(feature);
        }

    }
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
    layerList.append(newLayer);
    layerListModel->setLayerList(layerList);
}


void MainWindow::on_actionAdd_Pad_triggered()
{
    if (!m_activeLayer)
    {
        QMessageBox::warning(this, tr("Empty Layer"),"레이어가 없습니다", QMessageBox::Yes);
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
