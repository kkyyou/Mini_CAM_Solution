#ifndef CVIEW_H
#define CVIEW_H

#include "mainwindow.h"

#include <QWidget>
#include <QQueue>

class CFeature;
class CShape;
class CView : public QWidget
{
    Q_OBJECT
public:
    CView(QWidget *parent);

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    QMatrix getMatrix(QRect windowArea, QRect viewArea);
    void drawLayer(CLayer *layer, QPainter *painter, const QColor &penColor);

    MainWindow *getMainWindow() const;
    void setMainWindow(MainWindow *mainWindow);

    // Zoom.
    void zoomIn();
    void zoomOut();

    // Panning.
    void panning();

    // Draw Feature.
    void drawPad(CFeature *feature, QPainter *painter, const QColor &penColor);
    void drawLine(CFeature *feature, QPainter *painter, const QColor &penColor);

    void drawPadRound(const QPoint &centerPoint, const long &radius, QPainter *painter, const QColor &penColor);
    void drawPadRect(const QPoint &centerPoint, const long &width, const long &height, QPainter *painter, const QColor &penColor);
    void drawLineRect(const QPoint &startPoint , const QPoint &endPoint, const long &penWidth, QPainter *painter, const QColor &penColor);
    void drawLineRound(const QPoint &startPoint , const QPoint &endPoint, const long &penWidth, QPainter *painter, const QColor &penColor);

    void drawLinePreview(const QMap<QString, QVariant> &commandValueMap, QPainter *painter, const QColor &penColor);

signals:
    void updateCurMousePositionSignal(long, long);

private:
    QRect m_viewArea;           // View 영역.
    MainWindow *m_mainWindow;
    QPixmap m_pixMap;
    QPoint m_pos;
    QPoint m_lastPos;

    double m_zoomInFactor;
    double m_zoomOutFactor;

    bool m_doPanning;
};

#endif // CVIEW_H
