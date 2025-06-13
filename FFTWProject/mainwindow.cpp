#include "mainwindow.h"
#include <QtCharts/QValueAxis>
#include <cmath>
#include "fftw_utils.h"

MainWindow::MainWindow(int N, double fs_Hz, QString f_unit, QString t_unit, fftw_complex *in, fftw_complex *out, QWidget *parent)
    : QWidget(parent)
{
    // --- 時間軸計算 ---
    QList<double> time_axis;
    time_axis.reserve(N);
    for (int i = 0; i < N; ++i)
    {
        double t = i / fs_Hz;                 // 秒
        time_axis.append(fromSec(t, t_unit)); // 表示単位に変換
    }

    // --- 周波数軸計算 ---
    int N_half = N / 2;
    QList<double> freq_axis;
    freq_axis.reserve(N_half + 1);
    for (int k = 0; k <= N_half; ++k)
    {
        double freq = k * fs_Hz / N;            // Hz
        freq_axis.append(fromHz(freq, f_unit)); // 表示単位に変換
    }

    // --- 時間波形 ---
    QtCharts::QLineSeries *series_time = new QtCharts::QLineSeries(this);
    double min_val = in[0][0], max_val = in[0][0];
    for (int i = 0; i < N; ++i)
    {
        series_time->append(time_axis[i], in[i][0]);
        if (in[i][0] < min_val)
            min_val = in[i][0];
        if (in[i][0] > max_val)
            max_val = in[i][0];
    }
    QtCharts::QChart *chart_time = new QtCharts::QChart();
    chart_time->addSeries(series_time);
    chart_time->setTitle("Input Signal (Time Domain)");
    chart_time->createDefaultAxes();
    chart_time->axes(Qt::Horizontal).first()->setTitleText("Time [" + t_unit + "]");
    chart_time->axes(Qt::Vertical).first()->setTitleText("Amplitude");
    auto yAxis_time = qobject_cast<QtCharts::QValueAxis *>(chart_time->axes(Qt::Vertical).first());
    double y_center = 0.5 * (max_val + min_val);
    double y_half = 0.55 * (max_val - min_val);
    if (y_half < 1e-6)
        y_half = 1.0;
    yAxis_time->setRange(y_center - y_half, y_center + y_half);

    // --- スペクトル ---
    QtCharts::QLineSeries *series_freq = new QtCharts::QLineSeries(this);
    const double R = 50.0;
    for (int k = 0; k <= N_half; ++k)
    {
        double mag = sqrt(out[k][0] * out[k][0] + out[k][1] * out[k][1]) * 2.0 / N;
        double power = (mag * mag) / R;
        double dBm = 10.0 * log10(power / 0.001 + 1e-20);
        series_freq->append(freq_axis[k], dBm);
    }
    QtCharts::QChart *chart_freq = new QtCharts::QChart();
    chart_freq->addSeries(series_freq);
    chart_freq->setTitle("FFT Spectrum (dBm)");
    chart_freq->createDefaultAxes();
    chart_freq->axes(Qt::Horizontal).first()->setTitleText("Frequency [" + f_unit + "]");
    chart_freq->axes(Qt::Vertical).first()->setTitleText("Power [dBm]");
    auto yAxis_freq = qobject_cast<QtCharts::QValueAxis *>(chart_freq->axes(Qt::Vertical).first());
    yAxis_freq->setRange(-100.0, 50.0);

    // --- ラベル ---
    label_time = new QLabel("X: , Y: ", this);
    label_freq = new QLabel("X: , Y: ", this);

    // --- ChartView ---
    chartView_time = new CrosshairChartView(chart_time, series_time, label_time, this);
    chartView_time->setRenderHint(QPainter::Antialiasing);

    chartView_freq = new CrosshairChartView(chart_freq, series_freq, label_freq, this);
    chartView_freq->setRenderHint(QPainter::Antialiasing);

    // --- リセットボタン ---
    resetButton = new QPushButton("Reset Zoom", this);
    connect(resetButton, &QPushButton::clicked, [=]()
            {
        chartView_time->chart()->zoomReset();
        chartView_freq->chart()->zoomReset(); });

    // --- レイアウト ---
    layout = new QVBoxLayout(this);
    layout->addWidget(resetButton);
    layout->addWidget(label_time);
    layout->addWidget(chartView_time);
    layout->addWidget(label_freq);
    layout->addWidget(chartView_freq);

    setWindowTitle("FFT Signal & Spectrum");
    resize(900, 750);
}