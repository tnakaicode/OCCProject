#include <cmath>
#include <iostream>
#include <fftw3.h>
#include <QApplication>
#include <QtCharts>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/point_generators_2.h>
#include <Eigen/Dense>
#include "fftw_utils.h"
#include "crosshairchartview.h"
#include "mainwindow.h"

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Point_2 p(1.0, 2.0);
    Point_2 q(4.0, 6.0);

    double dist = std::sqrt(CGAL::squared_distance(p, q));
    std::cout << "Distance: " << dist << std::endl;

    // 2x2行列の作成
    Eigen::Matrix2d mat;
    mat << 1, 2,
        3, 4;

    // ベクトルの作成
    Eigen::Vector2d vec(5, 6);

    // 行列とベクトルの積
    Eigen::Vector2d result = mat * vec;

    std::cout << "Matrix:\n"
              << mat << std::endl;
    std::cout << "Vector:\n"
              << vec << std::endl;
    std::cout << "Result:\n"
              << result << std::endl;

    // 固有値計算
    Eigen::EigenSolver<Eigen::Matrix2d> solver(mat);
    std::cout << "Eigenvalues:\n"
              << solver.eigenvalues() << std::endl;

    // サンプリング周波数
    const double fs = 10.0;
    QString fs_unit = "GHz";
    double fs_Hz = toHz(fs, fs_unit);

    // 周波数分解能
    const double df = 1;
    QString df_unit = "MHz";
    double df_Hz = toHz(df, df_unit);

    // 周波数分解能からFFTサイズを決定
    int N1 = static_cast<int>(fs_Hz / df_Hz);
    std::cout << "fs_Hz: " << fs_Hz << ", df_Hz: " << df_Hz << std::endl;
    std::cout << "N: " << N1 << std::endl;

    // FFTサイズ
    const int N = 1024 * 20;
    std::cout << "N: " << N << std::endl;

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
