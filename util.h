#pragma once

#include <random>
#include <chrono>
#include <vector>


double random_double(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distribution(min, max);

    return distribution(gen);
}

int random_int(int min, int max) {
    // Use a high-resolution clock to seed the random number engine
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(min, max);

    // Generate a random number within the specified range
    return distribution(generator);
}

bool random_bool()
{
    // Use a high-resolution clock to seed the random number engine
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1); // 0 represents false, 1 represents true

    // Generate a random integer (0 or 1) and convert it to bool
    return static_cast<bool>(distribution(generator));
}

std::vector<float> linspace(float startValue, float endValue, int numValues) {
    std::vector<float> result(numValues);
    float step = (endValue - startValue) / float(numValues - 1);
    for (int i = 0; i < numValues; ++i) {
        result[i] = startValue + float(i) * step;
    }
    return result;
}