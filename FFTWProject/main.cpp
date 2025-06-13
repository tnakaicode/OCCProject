#include <iostream>
#include <fftw3.h>
#include "fftw_utils.h"

int main()
{
    // Define the size of the FFT
    const int N = 8;

    // Allocate input and output arrays
    fftw_complex *in = allocate_complex_array(N);
    fftw_complex *out = allocate_complex_array(N);

    // Initialize input data
    for (int i = 0; i < N; ++i)
    {
        in[i][0] = static_cast<double>(i); // 実部
        in[i][1] = 0.0;                    // 虚部
    }

    // Create FFTW plan
    fftw_plan plan = create_fft_plan_1d(N, in, out, FFTW_FORWARD);

    // Execute the FFT
    execute_fft(plan, in, out);

    // Output the results
    std::cout << "FFT Output:" << std::endl;
    for (int i = 0; i < N; ++i)
    {
        std::cout << "(" << out[i][0] << ", " << out[i][1] << ")" << std::endl;
    }

    // Clean up
    destroy_fft_plan(plan);
    free_complex_array(in);
    free_complex_array(out);

    return 0;
}