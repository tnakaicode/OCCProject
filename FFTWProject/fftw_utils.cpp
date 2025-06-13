#include "fftw_utils.h"
#include <fftw3.h>
#include <iostream>
#include <vector>

void create_fft_plan(fftw_plan &plan, std::vector<double> &input, std::vector<fftw_complex> &output, int n) {
    plan = fftw_plan_dft_r2c_1d(n, input.data(), output.data(), FFTW_ESTIMATE);
}

void execute_fft(fftw_plan &plan) {
    fftw_execute(plan);
}

void destroy_fft_plan(fftw_plan &plan) {
    fftw_destroy_plan(plan);
}

void allocate_memory(std::vector<double> &input, std::vector<fftw_complex> &output, int n) {
    input.resize(n);
    output.resize(n / 2 + 1); // FFTW stores half of the complex output for real input
}

void free_memory(std::vector<double> &input, std::vector<fftw_complex> &output) {
    input.clear();
    output.clear();
}