#include <vector>
#include <iostream>

#include "simulator.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

const int FRAME_WIDTH = 1000;
const int FRAME_HEIGHT = 1000;
const int iterations = 3000;
const int N = 1000;

// TODO: Parameterize mean and sigma in function definition
void generate_gaussian_signal(std::vector<float>& gaussian, float mean_min, float mean_max, float sigma_min, float sigma_max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate random mean in the range of 0 to N
    std::uniform_real_distribution<float> mean_dist(mean_min, mean_max);
    float mean = mean_dist(gen);

    // Generate random sigma (standard deviation)
    std::uniform_real_distribution<float> sigma_dist(sigma_min, sigma_max);
    float sigma = sigma_dist(gen);

    // Generate Gaussian signal
    for (int i = 0; i < N; ++i) {
        float x = static_cast<float>(i) - mean;
        gaussian[i] = std::exp(-(x * x) / (2.0f * sigma * sigma));
    }
}

void add_signal(std::vector<float>& signal, std::vector<float>& add_signal)
{
    for (int i = 0; i < N; i++)
    {
        signal[i] += add_signal[i];
    }
}

void factor_signal(std::vector<float>& signal, float factor)
{
    for (int i = 0; i < N; i++)
    {
        signal[i] *= factor;
    }
}

void reshift(std::vector<float>& signal)
{
    float shift = -signal[0];
    for (int i = 0; i < N; i++)
    {
        signal[i] += shift;
    }
}

int main()
{
    std::vector<float> indices(N);
    for (int i = 0; i < N; i++)
    {
        indices[i] = float(i);
    }

    std::vector<float> iteration_indices(iterations);
    for (int i = 0; i < iterations; i++)
    {
        iteration_indices[i] = float(i);
    }

    std::vector<float> signal_offset_x(N);
    std::vector<float> signal_offset_y(N);
    std::vector<float> signal_yaw(N);

    // Filling initial signal guesses
    for (int i = 0; i < N; i++)
    {
        signal_offset_x[i] = 0.0f;
        signal_offset_y[i] = 0.0f;
    }

    signal_yaw = linspace(0.0f, 90.0f * M_PI / 180.0f, N);

    Simulator simulator(N, FRAME_WIDTH, FRAME_HEIGHT);

    int score = simulator.simulate(signal_yaw, signal_offset_x, signal_offset_y);

    std::vector<float> gaussian_signal(N);
    std::vector<float> scores(iterations);

    float mean_min = 0.0f;
    float mean_max = float(N);
    float sigma_min = 500.0f;
    float sigma_max = float(N) * 2;

    float lower_factor_bound = -0.1f;
    float upper_factor_bound = 0.1f;

    int total_pixel = FRAME_WIDTH * FRAME_HEIGHT;

    for (int i = 0; i < iterations; i++)
    {
        std::vector<float> new_signal_offset_x = signal_offset_x;
        std::vector<float> new_signal_offset_y = signal_offset_y;

        generate_gaussian_signal(gaussian_signal, mean_min, mean_max, sigma_min, sigma_max);
        factor_signal(gaussian_signal, random_float(lower_factor_bound, upper_factor_bound));
        add_signal(new_signal_offset_x, gaussian_signal);
        reshift(new_signal_offset_x);

        generate_gaussian_signal(gaussian_signal, mean_min, mean_max, sigma_min, sigma_max);
        factor_signal(gaussian_signal, random_float(lower_factor_bound, upper_factor_bound));
        add_signal(new_signal_offset_y, gaussian_signal);
        reshift(new_signal_offset_y);

        int new_score = simulator.simulate(signal_yaw, new_signal_offset_x, new_signal_offset_y);

        if (new_score > score)
        {
            // TODO:  store gaussian signal into some kind of list
            score = new_score;
            signal_offset_x = new_signal_offset_x;
            signal_offset_y = new_signal_offset_y;
            SDL_GL_SwapWindow(simulator.window);
        }

        scores[i] = score;
    }

    

    std::cout << score << std::endl;
    float fraction = float(score) / float(total_pixel);
    float area = fraction * 16.0f;
    std::cout << area << std::endl;

    simulator.sleep(2000);

    plt::plot(indices, signal_yaw);
    plt::show();
    plt::plot(indices, signal_offset_x);
    plt::show();
    plt::plot(indices, signal_offset_y);
    plt::show();

    plt::plot(iteration_indices, scores);
    plt::show();
}