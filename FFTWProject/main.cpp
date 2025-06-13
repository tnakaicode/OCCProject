#include <QApplication>
#include <QtCharts>
#include <QVBoxLayout>
#include <QWidget>
#include <fftw3.h>
#include "fftw_utils.h"
#include <cmath>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const int N = 1024;
    const double fs = 100000.0; // サンプリング周波数 100kHz
    const double f = 10000.0;   // 信号周波数 10kHz

    fftw_complex *in = allocate_complex_array(N);
    fftw_complex *out = allocate_complex_array(N);

    // 10kHzサイン波を入力
    for (int i = 0; i < N; ++i)
    {
        double t = i / fs;
        in[i][0] = sin(2.0 * M_PI * f * t); // 実部
        in[i][1] = 0.0;                     // 虚部
    }

    // FFT
    fftw_plan plan = create_fft_plan_1d(N, in, out, FFTW_FORWARD);
    execute_fft(plan, in, out);

    // --- 時間波形（上） ---
    QtCharts::QLineSeries *series_time = new QtCharts::QLineSeries();
    for (int i = 0; i < N; ++i)
    {
        double t = i / fs * 1000.0; // ms表示
        series_time->append(t, in[i][0]);
    }
    QtCharts::QChart *chart_time = new QtCharts::QChart();
    chart_time->addSeries(series_time);
    chart_time->setTitle("Input Signal (Time Domain)");
    chart_time->createDefaultAxes();
    chart_time->axes(Qt::Horizontal).first()->setTitleText("Time [ms]");
    chart_time->axes(Qt::Vertical).first()->setTitleText("Amplitude");
    QtCharts::QChartView *chartView_time = new QtCharts::QChartView(chart_time);
    chartView_time->setRenderHint(QPainter::Antialiasing);
    chartView_time->setRubberBand(QtCharts::QChartView::RectangleRubberBand);

    // --- スペクトル（下） ---
    QtCharts::QLineSeries *series_freq = new QtCharts::QLineSeries();
    int N_half = N / 2;
    for (int k = 0; k <= N_half; ++k)
    {
        double freq = k * fs / N;
        double mag = sqrt(out[k][0] * out[k][0] + out[k][1] * out[k][1]) * 2.0 / N;
        series_freq->append(freq, mag);
    }
    QtCharts::QChart *chart_freq = new QtCharts::QChart();
    chart_freq->addSeries(series_freq);
    chart_freq->setTitle("FFT Spectrum (Amplitude)");
    chart_freq->createDefaultAxes();
    chart_freq->axes(Qt::Horizontal).first()->setTitleText("Frequency [Hz]");
    chart_freq->axes(Qt::Vertical).first()->setTitleText("Amplitude");
    QtCharts::QChartView *chartView_freq = new QtCharts::QChartView(chart_freq);
    chartView_freq->setRenderHint(QPainter::Antialiasing);
    chartView_freq->setRubberBand(QtCharts::QChartView::RectangleRubberBand);

    // --- レイアウト ---
    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);
    layout->addWidget(chartView_time);
    layout->addWidget(chartView_freq);

    window.setWindowTitle("FFT Signal & Spectrum");
    window.resize(900, 700);
    window.show();

    destroy_fft_plan(plan);
    free_complex_array(in);
    free_complex_array(out);

    return app.exec();
}