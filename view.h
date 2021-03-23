#ifndef CVIEW_H
#define CVIEW_H

#include "mainwindow.h"

#include <QWidget>

class CView : public QWidget
{
    Q_OBJECT
public:
    CView(QWidget *parent);

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QMatrix getMatrix(QRect windowArea, QRect viewArea);
    void drawLayer(CLayer *layer, QPainter *painter);

    void drawPadRound(QPoint centerPoint,long radius,QPainter *painter,QColor penColor);

signals:
    void updateCurMousePositionSignal(long, long);

private:
    QRect m_viewArea;           // View 영역.
    MainWindow *m_mainWindow;

    QPixmap m_pixMap;

    QPoint m_pos;
};

#endif // CVIEW_H
