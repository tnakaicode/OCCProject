#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include "crosshairchartview.h"
#include <fftw3.h>



class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(int N, double fs, QString f_unit, QString t_unit, fftw_complex *in, fftw_complex *out, QWidget *parent = nullptr);

private:
    QLabel *label_time;
    QLabel *label_freq;
    CrosshairChartView *chartView_time;
    CrosshairChartView *chartView_freq;
    QPushButton *resetButton;
    QVBoxLayout *layout;
};