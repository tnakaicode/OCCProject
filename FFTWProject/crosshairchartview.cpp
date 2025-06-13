#include "crosshairchartview.h"
#include <QMouseEvent>
#include <QPen>
#include <QValueAxis>
#include <QtCharts/QChart>

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

    xText = new QGraphicsSimpleTextItem();
    yText = new QGraphicsSimpleTextItem();
    xText->setBrush(Qt::blue);
    yText->setBrush(Qt::blue);
    chart->scene()->addItem(xText);
    chart->scene()->addItem(yText);
    xText->hide();
    yText->hide();
}

void CrosshairChartView::mouseMoveEvent(QMouseEvent *event) {
    QRectF plotArea = chart()->plotArea();
    if (plotArea.contains(event->pos())) {
        vLine->setLine(event->pos().x(), plotArea.top(), event->pos().x(), plotArea.bottom());
        hLine->setLine(plotArea.left(), event->pos().y(), plotArea.right(), event->pos().y());
        vLine->show();
        hLine->show();

        // グラフ座標に変換
        QPointF value = chart()->mapToValue(event->pos());

        // 軸をQValueAxisにキャスト
        auto xAxis = qobject_cast<QtCharts::QValueAxis*>(chart()->axisX());
        auto yAxis = qobject_cast<QtCharts::QValueAxis*>(chart()->axisY());

        // X座標値表示
        QString xStr = QString::number(value.x(), 'g', 6);
        xText->setText(xStr);
        if (xAxis)
            xText->setPos(chart()->mapToPosition(QPointF(value.x(), yAxis ? yAxis->min() : 0)) + QPointF(5, -20));
        else
            xText->setPos(event->pos() + QPointF(5, -20));
        xText->show();

        // Y座標値表示
        QString yStr = QString::number(value.y(), 'g', 6);
        yText->setText(yStr);
        if (yAxis)
            yText->setPos(chart()->mapToPosition(QPointF(xAxis ? xAxis->min() : 0, value.y())) + QPointF(5, -20));
        else
            yText->setPos(event->pos() + QPointF(5, -20));
        yText->show();
    } else {
        vLine->hide();
        hLine->hide();
        xText->hide();
        yText->hide();
    }
    QtCharts::QChartView::mouseMoveEvent(event);
}

void CrosshairChartView::leaveEvent(QEvent *event) {
    vLine->hide();
    hLine->hide();
    xText->hide();
    yText->hide();
    QtCharts::QChartView::leaveEvent(event);
}