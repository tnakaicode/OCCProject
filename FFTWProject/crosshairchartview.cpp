#include "crosshairchartview.h"
#include <QMouseEvent>
#include <QPen>
#include <QValueAxis>
#include <QtCharts/QChart>
#include <limits>

CrosshairChartView::CrosshairChartView(QtCharts::QChart *chart, QtCharts::QLineSeries *series, QLabel *label, QWidget *parent)
    : QtCharts::QChartView(chart, parent), m_series(series), m_label(label)
{
    setMouseTracking(true);
    setRubberBand(QtCharts::QChartView::RectangleRubberBand);

    auto scene = chart->scene();
    vLine = new QGraphicsLineItem(nullptr);
    hLine = new QGraphicsLineItem(nullptr);
    scene->addItem(vLine);
    scene->addItem(hLine);
    vLine->hide();
    hLine->hide();

    xText = new QGraphicsSimpleTextItem(nullptr);
    yText = new QGraphicsSimpleTextItem(nullptr);
    xText->setBrush(Qt::blue);
    yText->setBrush(Qt::blue);
    scene->addItem(xText);
    scene->addItem(yText);
    xText->hide();
    yText->hide();
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

        QPointF value = chart()->mapToValue(event->pos());
        auto xAxis = qobject_cast<QtCharts::QValueAxis *>(chart()->axisX());
        auto yAxis = qobject_cast<QtCharts::QValueAxis *>(chart()->axisY());

        QString xStr = QString::number(value.x(), 'g', 6);
        xText->setText(xStr);
        if (xAxis)
            xText->setPos(chart()->mapToPosition(QPointF(value.x(), yAxis ? yAxis->min() : 0)) + QPointF(5, -20));
        else
            xText->setPos(event->pos() + QPointF(5, -20));
        xText->show();

        QString yStr = QString::number(value.y(), 'g', 6);
        yText->setText(yStr);
        if (yAxis)
            yText->setPos(chart()->mapToPosition(QPointF(xAxis ? xAxis->min() : 0, value.y())) + QPointF(5, -20));
        else
            yText->setPos(event->pos() + QPointF(5, -20));
        yText->show();
    }
    else
    {
        vLine->hide();
        hLine->hide();
        xText->hide();
        yText->hide();
    }
    QtCharts::QChartView::mouseMoveEvent(event);
}

void CrosshairChartView::leaveEvent(QEvent *event)
{
    vLine->hide();
    hLine->hide();
    xText->hide();
    yText->hide();
    QtCharts::QChartView::leaveEvent(event);
}

void CrosshairChartView::mousePressEvent(QMouseEvent *event)
{
    if (!m_series || !m_label)
        return;

    // 最近傍のデータ点を探す
    QPointF value = chart()->mapToValue(event->pos());
    qreal minDist = std::numeric_limits<qreal>::max();
    QPointF nearestPt;
    int n = m_series->count();
    for (int i = 0; i < n; ++i)
    {
        QPointF pt = m_series->at(i);
        qreal dist = std::abs(pt.x() - value.x());
        if (dist < minDist)
        {
            minDist = dist;
            nearestPt = pt;
        }
    }
    // ラベルに表示
    m_label->setText(QString("X: %1, Y: %2").arg(nearestPt.x(), 0, 'g', 6).arg(nearestPt.y(), 0, 'g', 6));
    QtCharts::QChartView::mousePressEvent(event);
}