#include <iostream>
#include <fftw3.h>
#include "fftw_utils.h"

int main() {
    // Define the size of the FFT
    const int N = 8;
    
    // Allocate input and output arrays
    double* in = new double[N];
    fftw_complex* out = new fftw_complex[N];

    // Initialize input data
    for (int i = 0; i < N; ++i) {
        in[i] = static_cast<double>(i);
    }

    // Create FFTW plan
    fftw_plan plan = create_fft_plan(in, out, N);

    // Execute the FFT
    fftw_execute(plan);

    // Output the results
    std::cout << "FFT Output:" << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << "(" << out[i][0] << ", " << out[i][1] << ")" << std::endl;
    }

    // Clean up
    fftw_destroy_plan(plan);
    delete[] in;
    delete[] out;

    return 0;
}