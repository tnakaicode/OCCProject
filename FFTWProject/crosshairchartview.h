#pragma once
#include <QtCharts/QChartView>
#include <QGraphicsLineItem>

class CrosshairChartView : public QtCharts::QChartView
{
    Q_OBJECT
public:
    CrosshairChartView(QtCharts::QChart *chart, QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QGraphicsLineItem *vLine;
    QGraphicsLineItem *hLine;
};