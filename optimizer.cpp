#include <vector>
#include <iostream>

#include "simulator.h"

const int FRAME_WIDTH = 1400;
const int FRAME_HEIGHT = 1400;
const int N = 100;

void generate_gaussian_signal(std::vector<float>& gaussian)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate random mean in the range of 0 to N
    std::uniform_real_distribution<float> mean_dist(0.0f, static_cast<float>(N));
    float mean = mean_dist(gen);

    // Generate random sigma (standard deviation)
    std::uniform_real_distribution<float> sigma_dist(0.1f, 100.0f); // Mean = 1.0, Std dev = 0.5
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

int main()
{
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

    for (int i = 0; i < 1000; i++)
    {
        std::vector<float> new_signal_offset_x = signal_offset_x;
        std::vector<float> new_signal_offset_y = signal_offset_y;

        generate_gaussian_signal(gaussian_signal);
        factor_signal(gaussian_signal, random_float(-0.1, 0.1));
        add_signal(new_signal_offset_x, gaussian_signal);

        generate_gaussian_signal(gaussian_signal);
        factor_signal(gaussian_signal, random_float(-0.1, 0.1));
        add_signal(new_signal_offset_y, gaussian_signal);

        int new_score = simulator.simulate(signal_yaw, new_signal_offset_x, new_signal_offset_y);

        if (new_score > score)
        {
            score = new_score;
            signal_offset_x = new_signal_offset_x;
            signal_offset_y = new_signal_offset_y;

            std::cout << score << std::endl;
        }
    }
}