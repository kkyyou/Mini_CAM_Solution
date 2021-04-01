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
#include "featurelistmodel.h"
#include "savexmlfile.h"
#include "openxmlfile.h"
#include "addfeaturecommand.h"
#include "deletefeaturecommand.h"

#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_activeLayer(NULL),
      m_commandStep(0)
{
    ui->setupUi(this);

    m_job = new CJob();
    m_view = new CView(this);

    // 중앙에 Widget(드로우 영역) 배치.
    setCentralWidget(m_view);

    // Layer Model.
    CLayerListModel *layerListModel = new CLayerListModel(m_job, this);
    ui->layer_list_view->setModel(layerListModel);
    ui->layer_list_view->setColumnWidth(CLayerListModel::_COLUMN_ACTIVE, 60);
    ui->layer_list_view->setColumnWidth(CLayerListModel::_COLUMN_VIEW, 60);
    ui->layer_list_view->horizontalHeader()->setStretchLastSection(true);

    // Layer Model Delegate.
    CCheckBoxDelegate *activeLayerCheckBoxDelegate = new CCheckBoxDelegate(this);
    ui->layer_list_view->setItemDelegateForColumn(CLayerListModel::_COLUMN_ACTIVE, activeLayerCheckBoxDelegate);

    CCheckBoxDelegate *viewLayerCheckBoxDelegate = new CCheckBoxDelegate(this);
    ui->layer_list_view->setItemDelegateForColumn(CLayerListModel::_COLUMN_VIEW, viewLayerCheckBoxDelegate);

    // Feature Model.
    CFeatureListModel *featureListModel = new CFeatureListModel(this);
    ui->feature_list_view->setModel(featureListModel);
    ui->feature_list_view->setMinimumWidth(310);
    ui->feature_list_view->resizeColumnsToContents();
    ui->feature_list_view->horizontalHeader()->setStretchLastSection(true);

    // Connect.
    QObject::connect(layerListModel, SIGNAL(changeActiveLayer(CLayer *)), this, SLOT(setActiveLayer(CLayer *)));
    QObject::connect(this, SIGNAL(changedActiveLayerSignal(CLayer *)), featureListModel, SLOT(setActiveLayerSlot(CLayer *)));
    QObject::connect(this, SIGNAL(changedJob(CJob *)), layerListModel, SLOT(setJob(CJob *)));
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
    else if (m_command.compare(_SELECT_PT) == 0)
    {
        m_commandVarMap.insert(_CENTER_PT, point);
        run();
    }
    else if (m_command.compare(_SELECT_RECT) == 0)
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
    Q_UNUSED(point);

    if(m_commandStep == _STEP_1)
    {
        m_commandStep = _STEP_0;
        m_commandStep = _STEP_0;
    }
    else if(m_commandStep == _STEP_0)
    {
        m_command = _NO_COMMAND;
        m_commandShape = " ";
        ui->currentCommand->setText(m_command + m_commandShape);
    }
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
            feature->calcArea();
        }
        else if (m_commandShape.compare(_RECTANGLE) == 0)
        {
            qlonglong width = m_commandVarMap.value(_WIDTH).toLongLong();
            qlonglong height = m_commandVarMap.value(_HEIGHT).toLongLong();
            CShape *shape = new CRectangle(width, height);

            feature->setShape(shape);
            feature->calcArea();
        }

        addFeatureCommand(m_activeLayer, feature);
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
            feature->calcArea();
        }
        else if (m_commandShape.compare(_RECTANGLE) == 0)
        {
            qlonglong width = m_commandVarMap.value(_WIDTH).toLongLong();
            qlonglong height = m_commandVarMap.value(_HEIGHT).toLongLong();
            CShape *shape = new CRectangle(width, height);

            feature->setShape(shape);
            feature->calcArea();
        }

        addFeatureCommand(m_activeLayer, feature);
    }
    else if (m_command.compare(_SELECT_PT) == 0)
    {
        QPoint point = m_commandVarMap.value(_CENTER_PT).toPoint();

        int selectCnt = 0;
        QList<CFeature *> featureList = m_activeLayer->featureList();
        for (auto iterFeature = featureList.cbegin(); iterFeature != featureList.cend(); ++iterFeature)
        {
            CFeature *feature = *iterFeature;
            if (!feature)
                continue;

            QPainterPath areaPath = feature->getAreaPath();
            if (areaPath.contains(QPointF(point)))
            {
                selectCnt++;
                m_selectedFeatures.append(feature);
                break;
            }
        }

        if (selectCnt == 0)
            m_selectedFeatures.clear();
    }
    else if (m_command.compare(_SELECT_RECT) == 0)
    {
        QPoint start = m_commandVarMap.value(_START_PT).toPoint();
        QPoint end = m_commandVarMap.value(_END_PT).toPoint();

        int selectCnt = 0;
        QRect selectArea(start, end);
        QList<CFeature *> featureList = m_activeLayer->featureList();
        for (auto iterFeature = featureList.cbegin(); iterFeature != featureList.cend(); ++iterFeature)
        {
            CFeature *feature = *iterFeature;
            if (!feature)
                continue;

            QPainterPath areaPath = feature->getAreaPath();
            if (!areaPath.intersects(selectArea) && !areaPath.contains(selectArea))
                continue;

            m_selectedFeatures.append(feature);
            selectCnt++;
        }

        if (selectCnt == 0)
            m_selectedFeatures.clear();
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

void MainWindow::saveFile()
{
    if (!m_job)
        return;

    QString filename = QFileDialog::getSaveFileName(this, tr("Save"), QDir::currentPath(), tr("Documents (*.xml)") );
    QXmlStreamWriter xmlWriter;
    QFile file(filename + ".xml");

    if (!file.open(QIODevice::WriteOnly))
        return;

    // Write.
    CSaveXMLFile::saveJob(&xmlWriter, &file, m_job);
}

void MainWindow::addFeatureCommand(CLayer *layer, CFeature *feature)
{
    QUndoCommand *addCommand = new CAddFeatureCommand(layer, feature);
    m_undoStack.push(addCommand);
    ui->feature_list_view->resizeColumnsToContents();
}

void MainWindow::deleteFeatureCommand(CLayer *layer, CFeature *feature)
{
    QUndoCommand *deleteCommand = new CDeleteFeatureCommand(layer, feature);
    m_undoStack.push(deleteCommand);
}

void MainWindow::updatedFeatures()
{
    // Feature Model Update.
    CFeatureListModel *featureListModel = getFeatureListModel();
    featureListModel->setActiveLayerSlot(m_activeLayer);

    // View Repaint.
    m_view->repaint();
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
        newLayer->setFeatureColor(Qt::red);
        m_activeLayer = newLayer;

        QQueue<QPair<CLayer *, QColor>> *layerColor = m_job->getLayerAndColorQueue();
        QPair<CLayer *, QColor> dequeueLC = layerColor->dequeue();
        dequeueLC.first = newLayer;
        layerColor->enqueue(dequeueLC);

        emit changedActiveLayerSignal(newLayer);
    }

    layerList.append(newLayer);
    layerListModel->setLayerList(layerList);

    // Connect.
    QObject::connect(newLayer, SIGNAL(updatedFeatureListSignal(CLayer *)), this, SIGNAL(changedActiveLayerSignal(CLayer *)));
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
    // Active 변경되면 Select 취소.
    if (m_activeLayer != activeLayer)
    {
        m_selectedFeatures.clear();
    }

    m_activeLayer = activeLayer;
    m_view->repaint();
    emit changedActiveLayerSignal(activeLayer);
}

void MainWindow::resetJob()
{
    // model.
    CLayerListModel *layerListModel = getLayerListModel();
    if (layerListModel)
        layerListModel->setLayerList(QList<CLayer *>());

    CFeatureListModel *featureListModel = getFeatureListModel();
    if (featureListModel)
    {
        featureListModel->setActiveLayer(NULL);
        featureListModel->setFeatureList(QList<CFeature *>());
    }

    m_job->deleteLater();
    m_job = new CJob();
    m_activeLayer = NULL;
    m_view->setMainWindow(this);
    m_view->repaint();
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

CJob *MainWindow::job() const
{
    return m_job;
}

void MainWindow::setJob(CJob *job)
{
    m_job = job;
}

CLayerListModel *MainWindow::getLayerListModel()
{
    CLayerListModel *layerListModel = qobject_cast<CLayerListModel *>(ui->layer_list_view->model());
    return layerListModel;
}

CFeatureListModel *MainWindow::getFeatureListModel()
{
    CFeatureListModel *featureListModel = qobject_cast<CFeatureListModel *>(ui->feature_list_view->model());
    return featureListModel;
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

void MainWindow::on_actionSave_File_triggered()
{
    saveFile();
}

void MainWindow::on_actionOpen_File_triggered()
{
    // 현재 내용을 저장 할 지?
    if (m_activeLayer)
    {
        int result = QMessageBox::warning(this, tr("Save"), "Do you want to save the current content?", QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);

        if (result == QMessageBox::Cancel)      return;
        else if (result == QMessageBox::Ok)     saveFile();
    }

    // 현재 job 리셋.
    resetJob();

    QString filename = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("Documents (*.xml)"));
    QXmlStreamReader xmlReader;
    QFile file(filename);

    if (!file.open(QFile::ReadOnly))
        return;

    xmlReader.setDevice(&file);

    // Read File.
    COpenXMLFile::openJob(&xmlReader, m_job);

    m_view->repaint();

    emit changedJob(m_job);
}

void MainWindow::on_actionPoint_Select_triggered()
{
    m_command = _SELECT_PT;
    ui->currentCommand->setText("Command : " + m_command);
}

QList<CFeature *> MainWindow::getSelectedFeatures() const
{
    return m_selectedFeatures;
}

void MainWindow::setSelectedFeatures(const QList<CFeature *> &selectedFeatures)
{
    m_selectedFeatures = selectedFeatures;
}

void MainWindow::on_actionDelete_Feature_triggered()
{
    while (!m_selectedFeatures.isEmpty())
    {
        CFeature *feature = m_selectedFeatures.takeFirst();
        if (!feature)
            break;

        m_activeLayer->removeFeature(feature);
        deleteFeatureCommand(m_activeLayer, feature);
    }

    // update.
    updatedFeatures();
}

void MainWindow::on_actionUndo_triggered()
{
    m_undoStack.undo();

    // update.
    updatedFeatures();
}

void MainWindow::on_actionRedo_triggered()
{
    m_undoStack.redo();

    // update.
    updatedFeatures();
}

void MainWindow::on_actionRectangle_Select_triggered()
{
    m_command = _SELECT_RECT;
    ui->currentCommand->setText("Command : " + m_command);
}
