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

    // サンプリング周波数
    const double fs = 10.0;
    QString fs_unit = "GHz";
    double fs_Hz = toHz(fs, fs_unit);

    // 周波数分解能
    const double df = 1;
    QString df_unit = "MHz";
    double df_Hz = toHz(df, df_unit);

    // 周波数分解能からFFTサイズを決定
    // const int N = static_cast<int>(fs_Hz / 2 / df_Hz);

    // FFTサイズ
    const int N = 1024 * 2;

    // 信号周波数 f0
    const double f0 = 1.0;
    const double a0 = 1.0;   // 振幅
    const double phi0 = 0.0; // 位相
    QString f0_unit = "GHz";
    double f0_Hz = toHz(f0, f0_unit);

    // 信号周波数 f1
    const double f1 = 2.1;
    const double a1 = 1.5;   // 振幅
    const double phi1 = 0.0; // 位相
    QString f1_unit = "GHz";
    double f1_Hz = toHz(f1, f1_unit);

    fftw_complex *in = allocate_complex_array(N);
    fftw_complex *out = allocate_complex_array(N);

    // 10kHzサイン波を入力
    for (int i = 0; i < N; ++i)
    {
        double t = i / fs_Hz;
        in[i][0] = a0 * sin(2.0 * M_PI * f0_Hz * t + phi0) + a1 * sin(2.0 * M_PI * f1_Hz * t + phi1);
        in[i][1] = 0.0;
    }

    // FFT
    fftw_plan plan = create_fft_plan_1d(N, in, out, FFTW_FORWARD);
    execute_fft(plan, in, out);

    // 表示単位
    QString f_unit = "GHz";
    QString t_unit = "us";

    MainWindow window(N, fs_Hz, f_unit, t_unit, in, out);
    window.show();

    destroy_fft_plan(plan);
    free_complex_array(in);
    free_complex_array(out);

    return app.exec();
}
