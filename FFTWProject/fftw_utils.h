#ifndef FFTW_UTILS_H
#define FFTW_UTILS_H

#include <fftw3.h>

// Function to create a 1D FFT plan
fftw_plan create_fft_plan_1d(int n, fftw_complex *in, fftw_complex *out, int sign);

// Function to execute the FFT plan
void execute_fft(fftw_plan plan, fftw_complex *in, fftw_complex *out);

// Function to destroy the FFT plan
void destroy_fft_plan(fftw_plan plan);

// Function to allocate memory for complex data
fftw_complex* allocate_complex_array(int n);

// Function to free allocated memory
void free_complex_array(fftw_complex *array);

#endif // FFTW_UTILS_H