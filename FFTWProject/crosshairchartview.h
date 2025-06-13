#pragma once
#include <QtCharts/QChartView>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QMouseEvent>

class CrosshairChartView : public QtCharts::QChartView {
    Q_OBJECT
public:
    CrosshairChartView(QtCharts::QChart *chart, QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QGraphicsLineItem *vLine;
    QGraphicsLineItem *hLine;
    QGraphicsSimpleTextItem *xText;
    QGraphicsSimpleTextItem *yText;
};