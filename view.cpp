#include "view.h"
#include "typedefine.h"
#include "layer.h"
#include "feature.h"
#include "pad.h"
#include "line.h"
#include "job.h"

#include <QPainter>
#include <QMouseEvent>

// 좌표단위를 10에 백만을 곱하는 이유.
//  -> double값으로 연산을 하다보면 값의 오차가 많이 나므로
//  -> 10에 백만을 곱한 값을 단위로 계산한다.

CView::CView(QWidget *parent) :
    QWidget(parent),
    m_viewArea(-10 * _MILLION, -10 * _MILLION, 20 * _MILLION, 20 * _MILLION), // (-10 * 백만, -10 * 백만) 좌표에서 20 * 백만 크기의 Rect 생성.
    m_zoomInFactor(0.8),
    m_zoomOutFactor(1.2),
    m_doPanning(false)
{
    MainWindow *mainWindow = qobject_cast<MainWindow *>(parent);
    if (mainWindow)
    {
        m_mainWindow = mainWindow;
    }

    // 기본 색상이 검은색으로 보이도록 처리.
    QPalette pal;
    pal.setColor(QPalette::Background, Qt::black);

    setAutoFillBackground(true);
    setPalette(pal);
    setMouseTracking(true);

    // 마우스 이동 시 좌표 업데이트 -> 시그널 슬롯 연결.
    QObject::connect(this, SIGNAL(updateCurMousePositionSignal(long, long)), m_mainWindow, SLOT(updateCurMousePositionSlot(long, long)));
}

// 10에 백만 곱한 값을 윈도우픽셀값으로 변환해주는 매트릭스.
QMatrix CView::getMatrix(QRect windowArea, QRect viewArea)
{
    // 윈도우 영역 사이즈(픽셀)를 구한다.
    double windowHeight = windowArea.height();
    double windowWidth = windowArea.width();

    // 10의 백만 곱한 우리의 View Area.
    double viewAreaHeight = viewArea.height();
    double viewAreaWidth = viewArea.width();

    // 윈도우 영역을 View 영역으로 나눈 비율.
    double scaleX = (double)windowWidth / (double)viewAreaWidth;
    double scaleY = (double)windowHeight / (double)viewAreaHeight;

    // 절대값 씌우기.
    scaleX = qAbs(scaleX);
    scaleY = qAbs(scaleY);

    // Min : 윈도우의 Width / Height중 짧은 곳에 맞추겠다.
    double scale = qMax(scaleX, scaleY);

    QMatrix myMatrix;
    QMatrix matrix1, matrix2, matrix3, matrix4;

    qreal viewCenterX = (qreal)((qreal)viewArea.left() + (qreal)viewArea.right()) / (qreal)2;
    qreal viewCenterY = (qreal)((qreal)viewArea.top() + (qreal)viewArea.bottom()) / (qreal)2;

    // 화면 중심을 View Area의 Center로 이동.
    // 우리는 10에 백만 곱한 단위로 연산을 할것이므로, View Area의 중앙으로 이동해야
    // 다음 Scale 적용이 정상적으로 이루어진다.
    //! Qt 내부 버그?? View Area의 Left와 Right의 절대값이 같은 값이어야 하는데 미세한 오차가나서.
    //! viewCenterX,Y값이 음수가 나오므로 - 붙임.
    matrix1.translate(-viewCenterX, -viewCenterY);

    // Scale 적용.
    matrix2.scale(scale, scale);

    // Y값 반전. (Qt에서 위 방향이 -Y 임).
    matrix3.scale(1.0, -1.0);

    // 데이터 중심으로 이동.
    matrix4.translate(windowArea.center().x(), windowArea.center().y());

    // Matrix 적용하여 반환.
    myMatrix *= matrix1;
    myMatrix *= matrix2;
    myMatrix *= matrix3;
    myMatrix *= matrix4;

    return myMatrix;
}

void CView::drawLayer(CLayer *layer, QPainter *painter, const QColor &penColor)
{
    QList<CFeature *> featureList = layer->featureList();
    if (featureList.isEmpty())
        return;

    for (auto iter = featureList.cbegin(); iter != featureList.cend(); ++iter)
    {
        CFeature *feature = *iter;
        if (!feature)
            continue;

        // Feature Type별 드로우.
        switch (feature->type())
        {
        case _FEATURE_PAD:  drawPad(feature, painter, penColor);    break;
        case _FEATURE_LINE: drawLine(feature, painter, penColor);   break;
        default:                                                   break;
        }
    }
}

void CView::zoomIn()
{
    long zoomInViewWidth = m_viewArea.width() * m_zoomInFactor;
    long zoomInViewHeight = m_viewArea.height() * m_zoomInFactor;
    long zoomInViewWidthHalf = zoomInViewWidth / 2;
    long zoomInViewHeightHalf = zoomInViewHeight / 2;

    long left   = m_viewArea.left();
    long right  = m_viewArea.right();
    long top    = m_viewArea.bottom();
    long bottom = m_viewArea.top();

    long centerX = ((double)((double)left/2.0) + (double)((double)right/2.0));
    long centerY = ((double)((double)top/2.0) + (double)((double)bottom/2.0));

    QPoint center(centerX, centerY);

    // 내 마우스를 중심으로 ZoomIn한 viewAreaNew 구하기.
    QPoint topLeft = QPoint(m_pos.x() - zoomInViewWidthHalf, m_pos.y() - zoomInViewHeightHalf);
    QPoint bottomRight = QPoint(m_pos.x() + zoomInViewWidthHalf, m_pos.y() + zoomInViewHeightHalf);
    QRect viewAreaNew = QRect(topLeft, bottomRight);

    // 마우스 위치쪽으로 ZoomIn을 하기위해 현재 뷰의 Center와 마우스의 오프셋 만큼 이동.
    viewAreaNew.translate((center.x() - m_pos.x()) * m_zoomInFactor, (center.y() - m_pos.y()) * m_zoomInFactor);
    m_viewArea.setRect(viewAreaNew.left(), viewAreaNew.top(), viewAreaNew.width(), viewAreaNew.height());
    repaint();
}

void CView::zoomOut()
{
    long zoomOutViewWidth = m_viewArea.width() * m_zoomOutFactor;
    long zoomOutViewHeight = m_viewArea.height() * m_zoomOutFactor;
    long zoomOutViewWidthHalf = zoomOutViewWidth / 2;
    long zoomOutViewHeightHalf = zoomOutViewHeight / 2;

    long left   = m_viewArea.left();
    long right  = m_viewArea.right();
    long top    = m_viewArea.bottom();
    long bottom = m_viewArea.top();

    long centerX = ((double)((double)left/2.0) + (double)((double)right/2.0));
    long centerY = ((double)((double)top/2.0) + (double)((double)bottom/2.0));

    QPoint center(centerX, centerY);

    // 내 마우스를 중심으로 ZoomOut한 viewAreaNew 구하기.
    QPoint topLeft = QPoint(m_pos.x() - zoomOutViewWidthHalf, m_pos.y() - zoomOutViewHeightHalf);
    QPoint bottomRight = QPoint(m_pos.x() + zoomOutViewWidthHalf, m_pos.y() + zoomOutViewHeightHalf);
    QRect viewAreaNew = QRect(topLeft, bottomRight);

    // 마우스 위치쪽으로 Zoom Out을 하기위해 현재 뷰의 Center와 마우스의 오프셋 만큼 이동.
    viewAreaNew.translate((center.x() - m_pos.x()) * m_zoomOutFactor, (center.y() - m_pos.y()) * m_zoomOutFactor);
    m_viewArea.setRect(viewAreaNew.left(), viewAreaNew.top(), viewAreaNew.width(), viewAreaNew.height());
    repaint();
}

void CView::zoomInCurrentView()
{
    m_viewArea.setRect(m_viewArea.left() * m_zoomInFactor , m_viewArea.top() * m_zoomInFactor ,m_viewArea.width() * m_zoomInFactor,m_viewArea.height() * m_zoomInFactor);
    repaint();
}

void CView::zoomOutCurrentView()
{
    m_viewArea.setRect(m_viewArea.left() * m_zoomOutFactor, m_viewArea.top() * m_zoomOutFactor,m_viewArea.width() * m_zoomOutFactor,m_viewArea.height() * m_zoomOutFactor);
    repaint();
}

void CView::panning()
{
    long offsetX = m_lastPos.x() - m_pos.x();
    long offsetY = m_lastPos.y() - m_pos.y();

    // 오프셋 값만큼 View Rect 이동, View 너비/높이는 동일.
    //! getMatrix 함수에서 veiwCenter의 -를 붙여서 translate했기때문에 offset 값이 음/양 반대로 나오므로.
    //! 논리적으론 offset을 - 해줘야 맞지만 + 해줌 -> getMatrix 함수 추후 확인이 필요함.. 어렵다.
    m_viewArea.setRect(m_viewArea.left() + offsetX, m_viewArea.top() + offsetY, m_viewArea.width(), m_viewArea.height());
}

void CView::drawPad(CFeature *feature, QPainter *painter, const QColor &penColor)
{
    if (!feature)   return;

    CPad *pad = dynamic_cast<CPad *>(feature);
    if (!pad)
        return;

    CShape *shape = feature->shape();
    if (!shape)
        return;

    SHAPE_TYPE shapeType = shape->type();
    QPoint center = pad->getCenterPoint();
    if (shapeType == _SHAPE_ROUND)
    {
        long radius = shape->getRadius();
        drawPadRound(center, radius, painter, penColor);
    }
    else if (shapeType == _SHAPE_RECT)
    {
        long width = shape->getWidth();
        long height = shape->getHeight();
        drawPadRect(center, width, height, painter, penColor);
    }
}

void CView::drawLine(CFeature *feature, QPainter *painter, const QColor &penColor)
{
    if (!feature)   return;

    CLine *line = dynamic_cast<CLine *>(feature);
    if (!line)
        return;

    CShape *shape = feature->shape();
    if (!shape)
        return;

    QPoint start = line->getStartPoint();
    QPoint end = line->getEndPoint();

    SHAPE_TYPE shapeType = shape->type();
    if (shapeType == _SHAPE_ROUND)
    {
        long diameter = shape->getWidth();
        drawLineRound(start, end, diameter, painter, penColor);
    }
    else if (shapeType == _SHAPE_RECT)
    {
        long width = shape->getWidth();
        drawLineRect(start, end, width, painter, penColor);
    }
}

void CView::drawPadRound(const QPoint &centerPoint, const long &radius, QPainter *painter, const QColor &penColor)
{
    QPen pen;

    long penSize = radius * 2;

    pen.setColor(penColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(penSize);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);

    painter->drawPoint(centerPoint);
}

void CView::drawPadRect(const QPoint &centerPoint, const long &width, const long &height, QPainter *painter, const QColor &penColor)
{
    QPen pen;

    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::SolidLine);
    painter->setBrush(penColor);
    painter->setPen(pen);

    long leftTopX = centerPoint.x() - (width / 2);
    long leftTopY = centerPoint.y() - (height / 2);

    painter->drawRect(leftTopX, leftTopY, width, height);
}

void CView::drawLineRect(const QPoint &startPoint, const QPoint &endPoint, const long &penWidth, QPainter *painter, const QColor &penColor)
{
    QPen pen;

    pen.setColor(penColor);
    pen.setCapStyle(Qt::SquareCap);
    pen.setWidth(penWidth);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);

    painter->drawLine(startPoint, endPoint);
}

void CView::drawLineRound(const QPoint &startPoint, const QPoint &endPoint, const long &penWidth, QPainter *painter, const QColor &penColor)
{
    QPen pen;

    pen.setColor(penColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(penWidth);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);

    painter->drawLine(startPoint, endPoint);
}

void CView::drawLinePreview(QPainter *painter, const QColor &penColor)
{
    QMap<QString, QVariant> commandValueMap = m_mainWindow->commandVarMap();

    QPoint start = commandValueMap.value(_START_PT).toPoint();
    QString shapeStr = m_mainWindow->commandShape();

    // 라인의 스타트 점을 찍었을때 현재 마우스 위치로 프리뷰를 보여준다.
    if (shapeStr.compare(_ROUND) == 0)
    {
        long diameter = commandValueMap.value(_DIAMETER).toLongLong();
        drawLineRound(start, m_pos, diameter, painter, penColor);
    }
    else if (shapeStr.compare(_RECTANGLE) == 0)
    {
        long width = commandValueMap.value(_WIDTH).toLongLong();
        drawLineRect(start, m_pos, width, painter, penColor);
    }
}

void CView::drawSelectRectPreview(QPainter *painter, const QColor &penColor)
{
    QPen pen;

    pen.setColor(penColor);
    pen.setCapStyle(Qt::SquareCap);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(0);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(pen);

    QMap<QString, QVariant> commandValueMap = m_mainWindow->commandVarMap();
    QPoint start = commandValueMap.value(_START_PT).toPoint();
    QRect rect(start, m_pos);
    painter->drawRect(rect);
}

void CView::drawFeatureAreaPreview(QPainter *painter, const QColor &penColor)
{
    QList<CFeature *> features = m_mainWindow->activeLayer()->featureList();
    for (auto iterF = features.cbegin(); iterF != features.cend(); ++iterF)
    {
        CFeature *feature = *iterF;
        if (!feature)
            continue;

        QPainterPath areaPath = feature->getAreaPath();
        if (areaPath.contains(m_pos))
        {
            QPen pen;
            pen.setStyle(Qt::DashDotLine);
            pen.setWidth(50000);
            pen.setBrush(penColor);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::RoundJoin);
            painter->setPen(pen);
            painter->drawPath(areaPath);
        }
    }
}

MainWindow *CView::getMainWindow() const
{
    return m_mainWindow;
}

void CView::setMainWindow(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
}

void CView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // 픽스맵 사이즈 안맞을시 매칭.
    if (m_pixMap.size() != this->size())
    {
        m_pixMap = QPixmap(this->size());
    }

    m_pixMap.fill(Qt::transparent);

    QMatrix matrix = getMatrix(rect(), m_viewArea);
    QPainter painterPixmap;

    // 픽스맵에 드로우.
    painterPixmap.begin(&m_pixMap);
    painterPixmap.setMatrix(matrix);

    // 레이어 돌면서 존재하는 Feature 그리기.
    CJob *job = m_mainWindow->job();
    QList<CLayer *> viewLayerList = job->getViewLayerList();
    for (auto iter = viewLayerList.cbegin(); iter != viewLayerList.cend(); ++iter)
    {
        CLayer *viewLayer = *iter;
        if (!viewLayer)
            continue;

        drawLayer(viewLayer, &painterPixmap, viewLayer->featureColor());
    }

    int curCommandStep = m_mainWindow->commandStep();
    QString curCommand = m_mainWindow->command();

    // Line Preview.
    if (curCommand.compare(_ADD_LINE) == 0 && curCommandStep == _STEP_1)
    {
        drawLinePreview(&painterPixmap, m_mainWindow->activeLayer()->featureColor());
    }

    // Select Rectangle Preview.
    if (curCommand.compare(_SELECT_RECT) == 0 && curCommandStep == _STEP_1)
    {
        drawSelectRectPreview(&painterPixmap, Qt::white);
    }

    // 마우스 도형에 갖다대었을때 영역 Preview.
    if (curCommand.compare(_SELECT_PT) == 0)
    {
        drawFeatureAreaPreview(&painterPixmap, Qt::yellow);
    }

    // Select.
    QList<CFeature *> selectFeatures = m_mainWindow->getSelectedFeatures();
    for (auto iterSelect = selectFeatures.cbegin(); iterSelect != selectFeatures.cend(); ++iterSelect)
    {
        CFeature *feature = *iterSelect;
        if (!feature)
            continue;

        // Feature Type별 드로우.
        switch (feature->type())
        {
        case _FEATURE_PAD:  drawPad(feature, &painterPixmap, Qt::white);   break;
        case _FEATURE_LINE: drawLine(feature, &painterPixmap, Qt::white);  break;
        default:                                                           break;
        }
    }

    painterPixmap.end();

    // View에 드로우.
    QPainter painter;
    painter.begin(this);

    painter.drawPixmap(0, 0, m_pixMap);
    painter.end();
}

void CView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_mainWindow->goToNext(m_pos);
    }
    else if (event->button() == Qt::RightButton)
    {
        m_mainWindow->goToPrev(m_pos);

        m_lastPos = m_pos;
        m_doPanning = true;
    }
}

void CView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        m_doPanning = false;
    }
}

void CView::mouseMoveEvent(QMouseEvent *event)
{
    QMatrix matrix = getMatrix(rect(), m_viewArea);
    matrix = matrix.inverted();
    m_pos = matrix.map(event->pos());

    long long x = m_pos.x();
    long long y = m_pos.y();

    if (m_doPanning)
    {
        panning();
    }

    emit updateCurMousePositionSignal(x, y);
    repaint();
}

void CView::wheelEvent(QWheelEvent *event)
{
    QPoint scrollAmount = event->angleDelta();

    if (scrollAmount.y() > 0)
    {
        zoomIn();
    }
    else
    {
        zoomOut();
    }
}
