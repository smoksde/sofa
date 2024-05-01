#include <iostream>
#include <vector>
#include <random>

#include "matplotlibcpp.h"
#include "util.h"

#include "fourier.h"

namespace plt = matplotlibcpp;

int main()
{
    const int N = 1000; // time resolution

    std::vector<double> signal(N);
    std::vector<double> indices(N);
    for (int i = 0; i < N; i++)
    {
        signal[i] = sin(2 * PI * i / N) + 0.5 * sin(4 * PI * i / N);
        indices[i] = (double)i;
    }

    // Applying noise to signal
    for (int i = 0; i < N; i++)
    {
        signal[i] += random_double(-0.1, 0.1);
    }

    // plt::plot(indices, signal);

    std::complex<double> spectrum[N];
    DFT(signal, spectrum, N);

    for (int i = 0; i < N; i++)
    {
        std::cout << "Frequency: " << i << ", Magnitude: " << std::abs(spectrum[i]) << std::endl;
    }

    // Cutting higher frequency information
    for (int i = 0; i < N; i++)
    {
        if (i > 10)
        {
            spectrum[i] = 0.0;
        }   
    }

    std::vector<double> reconstructed_signal(N);
    IDFT(spectrum, reconstructed_signal, N);

    // plt::plot(indices, reconstructed_signal);
    // plt::show();

    plt::plot(indices, signal,"x");
    plt::show();
    plt::plot(indices, reconstructed_signal);
    plt::show();

    for (int i = 0; i < N; i++)
    {
        std::cout << "Reconstructed signal[" << i << "]: " << reconstructed_signal[i] << std::endl;
    }

    return 0;
}