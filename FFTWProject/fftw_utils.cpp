#include "fftw_utils.h"
#include <cstdlib>
#include <QString>

double toHz(double value, const QString &unit)
{
    if (unit == "Hz")
        return value;
    else if (unit == "kHz")
        return value * 1e3;
    else if (unit == "MHz")
        return value * 1e6;
    else if (unit == "GHz")
        return value * 1e9;
    else
        return value; // 未知の単位はそのまま
}

double fromHz(double value, const QString &unit)
{
    if (unit == "Hz")
        return value;
    else if (unit == "kHz")
        return value * 1e-3;
    else if (unit == "MHz")
        return value * 1e-6;
    else if (unit == "GHz")
        return value * 1e-9;
    else
        return value; // 未知の単位はそのまま
}

double toSec(double value, const QString &unit)
{
    if (unit == "s")
        return value;
    else if (unit == "ms")
        return value * 1e-3;
    else if (unit == "us")
        return value * 1e-6;
    else if (unit == "ns")
        return value * 1e-9;
    else
        return value; // 未知の単位はそのまま
}

double fromSec(double value, const QString &unit)
{
    if (unit == "s")
        return value;
    else if (unit == "ms")
        return value * 1e3;
    else if (unit == "us")
        return value * 1e6;
    else if (unit == "ns")
        return value * 1e9;
    else
        return value; // 未知の単位はそのまま
}

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