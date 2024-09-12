#include <iostream>
#include <vector>
#include <random>

#include "matplotlibcpp.h"
#include "util.h"

#include "fourier.h"

namespace plt = matplotlibcpp;

const int N = 1000;
const int population_size = 100;
const int iterations = 10000;

void generate_gaussian_signal(std::vector<double>& gaussian, double mean_min, double mean_max, double sigma_min, double sigma_max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate random mean in the range of 0 to N
    std::uniform_real_distribution<double> mean_dist(mean_min, mean_max);
    double mean = mean_dist(gen);

    // Generate random sigma (standard deviation)
    std::uniform_real_distribution<double> sigma_dist(sigma_min, sigma_max); // Mean = 1.0, Std dev = 0.5
    double sigma = sigma_dist(gen);

    // Generate Gaussian signal
    for (int i = 0; i < N; ++i) {
        double x = static_cast<double>(i) - mean;
        gaussian[i] = std::exp(-(x * x) / (2.0 * sigma * sigma));
    }
}

void add_signal(std::vector<double>& signal, std::vector<double>& add_signal)
{
    for (int i = 0; i < N; i++)
    {
        signal[i] += add_signal[i];
    }
}

void factor_signal(std::vector<double>& signal, double factor)
{
    for (int i = 0; i < N; i++)
    {
        signal[i] *= factor;
    }
}

double compute_score(std::vector<double>& candidate_signal, std::vector<double>& true_signal)
{
    double score = 0.0;
    for (int i = 0; i < N; i++)
    {
        score += std::pow(true_signal[i] - candidate_signal[i], 2);
    }
    return -score;
}

int main()
{
    std::vector<double> true_signal(N);
    std::vector<double> indices(N);
    
    // Creating the targeted signal
    for (int i = 0; i < N; i++)
    {
        true_signal[i] = sin(2 * PI * i / N) + 0.5 * sin(4 * PI * i / N);
        //true_signal[i] = -1.0;
        // true_signal[i] = 0.5 * sin(4 * PI * i / N);
        // true_signal[i] = i*i / 1000000.0;
        // true_signal[i] = sin(2 * PI * i / N) + cos(2 * PI * (i - 100) / N);
        indices[i] = (double)i;
    }

    plt::plot(indices, true_signal);
    plt::show();

    std::vector<double> best_signal(N);
    // Init guess
    for (int i = 0; i < N; i++)
    {
        best_signal[i] = 1.0;
    }

    plt::plot(indices, best_signal);
    plt::show();

    /*for (int i = 0; i < 10; i++)
    {
        std::vector<double> gaussian = best_signal;
        generate_gaussian_signal(gaussian);
        factor_signal(gaussian, random_double(-0.1, 0.1));
        plt::plot(indices, gaussian);
        plt::show();
    }*/

    std::vector<std::vector<double>> signal_progression;
    std::vector<double> score_progression;

    std::vector<double> gaussian_signal = best_signal;
    double best_score = compute_score(best_signal, true_signal);

    double mean_min = 0.0;
    double mean_max = double(N);
    double sigma_min = 0.1;
    double sigma_max = 100.0;

    double lower_factor_bound = -0.1;
    double upper_factor_bound = 0.1;

    for (int i = 0; i < iterations; i++)
    {
        std::vector<double> new_signal = best_signal;
        generate_gaussian_signal(gaussian_signal, mean_min, mean_max, sigma_min, sigma_max);
        factor_signal(gaussian_signal, random_double(lower_factor_bound, upper_factor_bound));
        add_signal(new_signal, gaussian_signal);
        double new_score = compute_score(new_signal, true_signal);
        if (new_score > best_score)
        {
            best_score = new_score;
            best_signal = new_signal;

            signal_progression.push_back(new_signal);
            score_progression.push_back(new_score);
        }
    }

    plt::plot(indices, true_signal);

    for (int j = 0; j < signal_progression.size(); j++)
    {
        plt::plot(indices, signal_progression[j]);
        plt::pause(0.05);
        std::cout << score_progression[j] << std::endl;
    }
    plt::show();

    plt::plot(indices, best_signal);
    plt::show();
    /*std::vector<std::vector<double>> population;

    for (int i = 0; i < iterations; i++)
    {
        population.clear();
        for (int j = 0; j < population_size; j++)
        {
            std::vector<double> new_signal = best_signal;
            population.push_back();
        }
    }*/
}