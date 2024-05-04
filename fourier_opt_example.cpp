#include <iostream>
#include <vector>
#include <random>

#include "matplotlibcpp.h"
#include "util.h"

#include "fourier.h"

namespace plt = matplotlibcpp;

const int N = 1000;

double compute_score(std::vector<double>& found_signal, std::vector<double>& true_signal, int n)
{
    double score = 0.0;
    for (int i = 0; i < n; i++)
    {
        score += std::pow(true_signal[i] - found_signal[i], 2);
    }
    return -score;
}

void mutate_magnitude(std::complex<double>& component, double mutation_range)
{
    double magnitude = std::abs(component);
    magnitude += mutation_range / 1000.0;
    double phase = std::arg(component);

    double magnitude_change = random_double(-mutation_range, mutation_range);
    magnitude *= magnitude_change;

    component = std::polar(magnitude, phase);
}

void mutate_phase(std::complex<double>& component, double mutation_range)
{
    double magnitude = std::abs(component);
    double phase = std::arg(component);

    double phase_change = random_double(-mutation_range, mutation_range);
    phase += phase_change;

    component = std::polar(magnitude, phase);
}

void mutate_magnitudes(std::vector<std::complex<double>>& spectrum, double mutation_rate, double mutation_range)
{
    int n = spectrum.size();
    for (int i = 0; i < n; i++)
    {
        std::complex<double> component = spectrum[i];
        if (random_double(0.0, 1.0) < mutation_rate)
        {
            double magnitude = std::abs(component);
            double angle = std::arg(component);

            double magnitude_change = random_double(-mutation_range, mutation_range);
            magnitude *= magnitude_change;

            // Convert back to rectangular coordinates
            component = std::polar(magnitude, angle);
        }
    }
}

void mutate_phases(std::vector<std::complex<double>>& spectrum, double mutation_rate, double mutation_range)
{
    for (auto& component : spectrum)
    {
        if (random_double(0.0, 1.0) < mutation_rate)
        {
            // Convert to polar coordinates
            double magnitude = std::abs(component);
            double angle = std::arg(component);

            // Add mutation to phase
            double phase_change = random_double(-mutation_range, mutation_range);
            angle += phase_change;

            // Convert back to rectangular coordinates
            component = std::polar(magnitude, angle);
        }
    }
}

void populate(std::vector<std::vector<std::complex<double>>>& spectrum_population, std::vector<std::complex<double>>& spectrum_template, int population_size)
{
    double mutation_rate = 0.2;
    double mutation_range = 0.5;
    
    for (int i = 0; i < population_size; i++)
    {
        std::vector<std::complex<double>> new_spectrum = spectrum_template;
        int index = random_int(0, 10);
        bool which = random_bool();
        if(which)
        {
            mutate_magnitude(new_spectrum[index], mutation_range);
        }
        else
        {
            mutate_phase(new_spectrum[index], mutation_range);
        }
        // mutate_magnitudes(new_spectrum, mutation_rate, mutation_range);
        // mutate_phases(new_spectrum, mutation_rate, mutation_range);
        spectrum_population.push_back(new_spectrum);
    }
}

int main()
{
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
        // true_signal[i] = 0.5 * sin(4 * PI * i / N);
        indices[i] = (double)i;
    }

    // Creating a different signal that is used as initial guess
    for (int i = 0; i < N; i++)
    {
        // found_signal[i] = cos(2 * PI * i / N);
        found_signal[i] = 2 * sin(4 * PI * i / N);
        // found_signal[i] = 1.0;
    }

    /*for (int i = 0; i < N; i++)
    {
        found_spectrum[i] = std::polar(random_double(0.0, 0.1),random_double(0.0, 0.1));
    }*/

    // IDFT(found_spectrum, found_signal, N);

    DFT(found_signal, found_spectrum, N);

    plt::plot(indices, true_signal, "x");
    plt::show();

    plt::plot(indices, found_signal, "x");
    plt::show();

    std::cout << "Beginning optimization algorithm" << std::endl;

    const int iterations = 10;
    const int population_size = 100;

    std::vector<std::vector<std::complex<double>>> spectrum_population(population_size, std::vector<std::complex<double>>(N));
    std::vector<std::vector<double>> signal_progression;

    for (int i = 0; i < iterations; i++)
    {
        std::cout << "Begin of iteration: " << i << std::endl;
        spectrum_population.clear();
        populate(spectrum_population, found_spectrum, population_size);

        for (int j = 0; j < population_size; j++)
        {
            std::vector<double> current_signal(N);
            IDFT(spectrum_population[j], current_signal, N);

            if (compute_score(true_signal, current_signal, N) > compute_score(true_signal, found_signal, N))
            {
                found_signal = current_signal;
                found_spectrum = spectrum_population[j];
                signal_progression.push_back(found_signal);
            }
        }
    }

    for (int j = 0; j < signal_progression.size(); j++)
    {
        plt::plot(indices, signal_progression[j]);
        std::cout << "Score after iteration " << j << ": " << compute_score(true_signal, signal_progression[j], N) << std::endl;
    }
    plt::show();

    // std::vector<double> reconstructed_signal(N);
    // IDFT(found_spectrum, reconstructed_signal, N);

    // plt::plot(indices, reconstructed_signal);
    // plt::show();

    plt::plot(indices, found_signal);
    plt::show();



    return 0;
}