#include "fourier.h"

const double PI = 3.14159265358979323846;

// Implementation of discrete fourier transformation
void DFT(std::vector<double>& signal, std::vector<std::complex<double>>& spectrum, int N)
{
    for (int k = 0; k < N; k++)
    {
        spectrum[k] = 0;
        for (int n = 0; n < N; n++)
        {
            spectrum[k] += std::complex<double>(signal[n], 0) * std::polar(1.0, -2 * PI * k * n / N);
        }
    }
}

// Implementation of inverse discrete fourier transformation
void IDFT(std::vector<std::complex<double>>& spectrum, std::vector<double>& signal, int N)
{
    for (int n = 0; n < N; n++)
    {
        signal[n] = 0;
        for (int k = 0; k < N; k++)
        {
            double angle = 2 * PI * k * n / N;
            signal[n] += (spectrum[k].real() / N) * cos(angle) - (spectrum[k].imag() / N) * sin(angle);
        }
    }
}

// Implementation of discrete fourier transformation
void DFT_float(std::vector<float>& signal, std::vector<std::complex<float>>& spectrum, int N)
{
    for (int k = 0; k < N; k++)
    {
        spectrum[k] = 0;
        for (int n = 0; n < N; n++)
        {
            spectrum[k] += std::complex<float>(signal[n], 0) * (std::complex<float>)std::polar(1.0, -2 * PI * k * n / N);
        }
    }
}

// Implementation of inverse discrete fourier transformation
void IDFT_float(std::vector<std::complex<float>>& spectrum, std::vector<float>& signal, int N)
{
    for (int n = 0; n < N; n++)
    {
        signal[n] = 0;
        for (int k = 0; k < N; k++)
        {
            double angle = 2 * PI * k * n / N;
            signal[n] += (spectrum[k].real() / N) * cos(angle) - (spectrum[k].imag() / N) * sin(angle);
        }
    }
}