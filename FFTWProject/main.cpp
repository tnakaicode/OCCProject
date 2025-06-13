#include <cmath>
#include <fftw3.h>
#include <QApplication>
#include <QtCharts>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include "fftw_utils.h"
#include "crosshairchartview.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const int N = 2048;          // FFTサイズ
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

    MainWindow window(N, fs, f, in, out);
    window.show();

    destroy_fft_plan(plan);
    free_complex_array(in);
    free_complex_array(out);

    return app.exec();
}
