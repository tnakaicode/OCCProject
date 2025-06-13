#pragma once
#include <QtCharts/QChartView>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QMouseEvent>
#include <QLabel>
#include <QtCharts/QLineSeries>

class CrosshairChartView : public QtCharts::QChartView {
    Q_OBJECT
public:
    CrosshairChartView(QtCharts::QChart *chart, QtCharts::QLineSeries *series = nullptr, QLabel *label = nullptr, QWidget *parent = nullptr);
    
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QGraphicsLineItem *vLine;
    QGraphicsLineItem *hLine;
    QGraphicsSimpleTextItem *xText;
    QGraphicsSimpleTextItem *yText;
    QtCharts::QLineSeries *m_series;
    QLabel *m_label;
};