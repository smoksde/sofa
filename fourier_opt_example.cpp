#include <iostream>
#include <vector>
#include <random>

#include "matplotlibcpp.h"
#include "util.h"

#include "fourier.h"

namespace plt = matplotlibcpp;

void mutate_magnitudes(std::vector<std::complex<double>>& spectrum, double mutation_rate, double mutation_range)
{
    for (auto& component : spectrum)
    {
        if (random_double(0.0, 1.0) < mutation_rate)
        {
            double magnitude_change = random_double(-mutation_range, mutation_range);
            component *= (1.0 + magnitude_change);
        }
    }
}

void mutate_phases(std::vector<std::complex<double>>& spectrum, double mutation_rate, double mutation_range)
{
    for (auto& component : spectrum)
    {
        if (random_double(0.0, 1.0) < mutation_rate)
        {
            double phase_change = random_double(-mutation_range, mutation_range);
            component *= std::polar(1.0, phase_change);
        }
    }
}

int main()
{

    const int N = 1000;
    std::vector<double> true_signal(N);
    std::vector<std::complex<double>> true_spectrum(N);
    std::vector<double> found_signal(N);
    std::vector<std::complex<double>> found_spectrum(N);

    std::vector<double> indices(N);

    std::vector<std::vector<double>> signal_list;

    // Creating the targeted signal
    for (int i = 0; i < N; i++)
    {
        true_signal[i] = sin(2 * PI * i / N) + 0.5 * sin(4 * PI * i / N);
        indices[i] = (double)i;
    }

    // Creating a different signal that is used as initial guess
    for (int i = 0; i < N; i++)
    {
        found_signal[i] = cos(2 * PI * i / N);
    }

    DFT(found_signal, found_spectrum, N);

    // Beginning optimization algorithm with found_spectrum

    const int iterations = 10;
    const int population_size = 100;

    std::vector<std::vector<std::complex>> spectrum_population(population_size, std::vector(N));

    std::vector<double> reconstructed_signal(N);
    IDFT(found_spectrum, reconstructed_signal, N);

    // plt::plot(indices, reconstructed_signal);
    // plt::show();

    plt::plot(indices, true_signal, "x");
    plt::show();
    plt::plot(indices, found_signal, "x");
    plt::show();

    return 0;
}