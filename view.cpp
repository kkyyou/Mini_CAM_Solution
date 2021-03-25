#include "view.h"
#include "typedefine.h"
#include "layer.h"
#include "feature.h"
#include "pad.h"
#include "line.h"

#include <QPainter>
#include <QMouseEvent>

// 좌표단위를 10에 백만을 곱하는 이유.
//  -> double값으로 연산을 하다보면 값의 오차가 많이 나므로
//  -> 10에 백만을 곱한 값을 단위로 계산한다.

CView::CView(QWidget *parent) :
    QWidget(parent),
    m_viewArea(-10 * _MILLION, -10 * _MILLION, 20 * _MILLION, 20 * _MILLION) // (-10 * 백만, -10 * 백만) 좌표에서 20 * 백만 크기의 Rect 생성.
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

QMatrix CView::getMatrix(QRect windowArea, QRect viewArea)
{
    // 윈도우 영역 사이즈(픽셀)를 구한다.
    double windowHeight = windowArea.height();
    double windowWidth = windowArea.width();

    // 10의 백만 곱한 우리의 View Area.
    double viewAreaHeight = viewArea.height();
    double viewAreaWidth = viewArea.width();

    // View Area에 대한 윈도우 영역의 비율을 구한다.
    // 우리 윈도우 영역에서 몇을 곱해줘야 View Area가 되냐.
    double scaleX = (double)windowWidth / (double)viewAreaWidth;
    double scaleY = (double)windowHeight / (double)viewAreaHeight;

    // 절대값 씌우기.
    scaleX = qAbs(scaleX);
    scaleY = qAbs(scaleY);

    // Min값 이유 : Max값으로하면 View Area영역이 넘침.
    double scale = qMin(scaleX, scaleY);

    QMatrix myMatrix;
    QMatrix matrix1, matrix2, matrix3, matrix4;

    qreal viewCenterX = (qreal)((qreal)viewArea.left() + (qreal)viewArea.right()) / (qreal)2;
    qreal viewCenterY = (qreal)((qreal)viewArea.top() + (qreal)viewArea.bottom()) / (qreal)2;

    // 화면 중심을 (0,0)으로 이동.
    // 우리는 10에 백만 곱한 단위로 연산을 할것이므로, View Area의 중앙으로 이동해야
    // 다음 Scale 적용이 정상적으로 이루어진다.
    matrix1.translate(viewCenterX, viewCenterY);

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

void CView::drawLayer(CLayer *layer, QPainter *painter)
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
        case _FEATURE_PAD:  drawPad(feature, painter, Qt::red);    break;
        case _FEATURE_LINE: drawLine(feature, painter, Qt::red);   break;
        default:                                                   break;
        }
    }
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
        long radius = shape->getRadius();
        drawLineRound(start, end, radius, painter, penColor);
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

void CView::drawLinePreview(const QMap<QString, QVariant> &commandValueMap, QPainter *painter, const QColor &penColor)
{
    QPoint start = commandValueMap.value(_START_PT).toPoint();
    QString shapeStr = m_mainWindow->commandShape();

    // 라인의 스타트 점을 찍었을때 현재 마우스 위치로 프리뷰를 보여준다.
    if (shapeStr.compare(_ROUND) == 0)
    {
        long radius = commandValueMap.value(_RADIUS).toLongLong();
        drawLineRound(start, m_pos, radius, painter, penColor);
    }
    else if (shapeStr.compare(_RECTANGLE) == 0)
    {
        long width = commandValueMap.value(_WIDTH).toLongLong();
        drawLineRect(start, m_pos, width, painter, penColor);
    }
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
    if (m_mainWindow->activeLayer())
    {
        drawLayer(m_mainWindow->activeLayer(), &painterPixmap);
        //drawSelectResult(&m_pixMap);
    }

    // 현재 커맨드 스텝에 따른 Feature 그리기.
    int curCommandStep = m_mainWindow->commandStep();
    QString curCommand = m_mainWindow->command();
    if (curCommand.compare(_ADD_LINE) == 0 && curCommandStep == _STEP_1)
    {
        QMap<QString, QVariant> commandValueMap = m_mainWindow->commandVarMap();
        drawLinePreview(commandValueMap, &painterPixmap, Qt::red);
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
    if (event->buttons() == Qt::LeftButton)
    {
        m_mainWindow->goToNext(m_pos);
    }
    else if (event->buttons() == Qt::RightButton)
    {
        m_mainWindow->goToPrev(m_pos);
    }
}

void CView::mouseMoveEvent(QMouseEvent *event)
{
    QMatrix matrix = getMatrix(rect(), m_viewArea);
    matrix = matrix.inverted();
    m_pos = matrix.map(event->pos());

    long long x = m_pos.x();
    long long y = m_pos.y();

    emit updateCurMousePositionSignal(x, y);
    repaint();
}
