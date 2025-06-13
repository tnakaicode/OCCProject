#include "crosshairchartview.h"
#include <QMouseEvent>
#include <QPen>

CrosshairChartView::CrosshairChartView(QtCharts::QChart *chart, QWidget *parent)
    : QtCharts::QChartView(chart, parent)
{
    setMouseTracking(true);
    setRubberBand(QtCharts::QChartView::RectangleRubberBand);

    vLine = new QGraphicsLineItem();
    hLine = new QGraphicsLineItem();
    vLine->setPen(QPen(Qt::red));
    hLine->setPen(QPen(Qt::red));
    chart->scene()->addItem(vLine);
    chart->scene()->addItem(hLine);
    vLine->hide();
    hLine->hide();
}

void CrosshairChartView::mouseMoveEvent(QMouseEvent *event)
{
    QRectF plotArea = chart()->plotArea();
    if (plotArea.contains(event->pos()))
    {
        vLine->setLine(event->pos().x(), plotArea.top(), event->pos().x(), plotArea.bottom());
        hLine->setLine(plotArea.left(), event->pos().y(), plotArea.right(), event->pos().y());
        vLine->show();
        hLine->show();
    }
    else
    {
        vLine->hide();
        hLine->hide();
    }
    QtCharts::QChartView::mouseMoveEvent(event);
}

void CrosshairChartView::leaveEvent(QEvent *event)
{
    vLine->hide();
    hLine->hide();
    QtCharts::QChartView::leaveEvent(event);
}