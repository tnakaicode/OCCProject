#include <QApplication>
#include <QtCharts>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <fftw3.h>
#include "fftw_utils.h"
#include "crosshairchartview.h"
#include <cmath>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const int N = 2048;
    const double fs = 1000000.0; // サンプリング周波数 1MHz
    const double f = 10000.0;    // 信号周波数 10kHz

    fftw_complex *in = allocate_complex_array(N);
    fftw_complex *out = allocate_complex_array(N);

    // 10kHzサイン波を入力
    for (int i = 0; i < N; ++i)
    {
        double t = i / fs;
        in[i][0] = sin(2.0 * M_PI * f * t);
        in[i][1] = 0.0;
    }

    // FFT
    fftw_plan plan = create_fft_plan_1d(N, in, out, FFTW_FORWARD);
    execute_fft(plan, in, out);

    // --- 時間波形（上） ---
    QtCharts::QLineSeries *series_time = new QtCharts::QLineSeries();
    double min_val = in[0][0], max_val = in[0][0];
    for (int i = 0; i < N; ++i)
    {
        double t = i / fs * 1000.0; // ms表示
        series_time->append(t, in[i][0]);
        if (in[i][0] < min_val)
            min_val = in[i][0];
        if (in[i][0] > max_val)
            max_val = in[i][0];
    }
    QtCharts::QChart *chart_time = new QtCharts::QChart();
    chart_time->addSeries(series_time);
    chart_time->setTitle("Input Signal (Time Domain)");
    chart_time->createDefaultAxes();
    chart_time->axes(Qt::Horizontal).first()->setTitleText("Time [ms]");
    chart_time->axes(Qt::Vertical).first()->setTitleText("Amplitude");
    auto yAxis_time = qobject_cast<QtCharts::QValueAxis *>(chart_time->axes(Qt::Vertical).first());
    double y_center = 0.5 * (max_val + min_val);
    double y_half = 0.55 * (max_val - min_val);
    if (y_half < 1e-6)
        y_half = 1.0;
    yAxis_time->setRange(y_center - y_half, y_center + y_half);

    CrosshairChartView *chartView_time = new CrosshairChartView(chart_time);
    chartView_time->setRenderHint(QPainter::Antialiasing);

    // --- スペクトル（下） ---
    QtCharts::QLineSeries *series_freq = new QtCharts::QLineSeries();
    int N_half = N / 2;
    const double R = 50.0;
    for (int k = 0; k <= N_half; ++k)
    {
        double freq = k * fs / N;
        double mag = sqrt(out[k][0] * out[k][0] + out[k][1] * out[k][1]) * 2.0 / N;
        double power = (mag * mag) / R;
        double dBm = 10.0 * log10(power / 0.001 + 1e-20);
        series_freq->append(freq, dBm);
    }
    QtCharts::QChart *chart_freq = new QtCharts::QChart();
    chart_freq->addSeries(series_freq);
    chart_freq->setTitle("FFT Spectrum (dBm)");
    chart_freq->createDefaultAxes();
    chart_freq->axes(Qt::Horizontal).first()->setTitleText("Frequency [Hz]");
    chart_freq->axes(Qt::Vertical).first()->setTitleText("Power [dBm]");
    auto yAxis_freq = qobject_cast<QtCharts::QValueAxis *>(chart_freq->axes(Qt::Vertical).first());
    yAxis_freq->setRange(-100.0, 50.0);

    CrosshairChartView *chartView_freq = new CrosshairChartView(chart_freq);
    chartView_freq->setRenderHint(QPainter::Antialiasing);

    // --- リセットボタン ---
    QPushButton *resetButton = new QPushButton("Reset Zoom");
    QObject::connect(resetButton, &QPushButton::clicked, [=]()
                     {
        chartView_time->chart()->zoomReset();
        chartView_freq->chart()->zoomReset(); });

    // --- レイアウト ---
    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(resetButton);
    layout->addWidget(chartView_time);
    layout->addWidget(chartView_freq);

    window.setWindowTitle("FFT Signal & Spectrum");
    window.resize(900, 750);
    window.show();

    destroy_fft_plan(plan);
    free_complex_array(in);
    free_complex_array(out);

    return app.exec();
}
