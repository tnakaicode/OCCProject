#include "fftw_utils.h"
#include <cstdlib>

fftw_plan create_fft_plan_1d(int n, fftw_complex *in, fftw_complex *out, int sign)
{
    return fftw_plan_dft_1d(n, in, out, sign, FFTW_ESTIMATE);
}

void execute_fft(fftw_plan plan, fftw_complex *in, fftw_complex *out)
{
    // in, outはplan作成時に指定済みなので、fftw_executeでOK
    fftw_execute(plan);
}

void destroy_fft_plan(fftw_plan plan)
{
    fftw_destroy_plan(plan);
}

fftw_complex *allocate_complex_array(int n)
{
    return (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
}

void free_complex_array(fftw_complex *array)
{
    fftw_free(array);
}