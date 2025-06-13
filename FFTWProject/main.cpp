#include <QApplication>
#include <QtCharts>
#include <fftw3.h>
#include "fftw_utils.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Define the size of the FFT
    const int N = 1024;

    // Allocate input and output arrays
    fftw_complex *in = allocate_complex_array(N);
    fftw_complex *out = allocate_complex_array(N);

    // Initialize input data
    for (int i = 0; i < N; ++i)
    {
        in[i][0] = static_cast<double>(i);
        in[i][1] = 0.0;
    }

    // Create FFTW plan
    fftw_plan plan = create_fft_plan_1d(N, in, out, FFTW_FORWARD);

    // Execute the FFT
    execute_fft(plan, in, out);

    // Qt Chartsでグラフ描画
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    for (int i = 0; i < N; ++i)
    {
        series->append(i, out[i][0]); // 実部のみ
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("FFT Real Part");
    chart->createDefaultAxes();

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(600, 400);
    window.show();

    destroy_fft_plan(plan);
    free_complex_array(in);
    free_complex_array(out);

    return app.exec();
}