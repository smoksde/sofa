#pragma once

#include <random>

double random_double(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distribution(min, max);

    return distribution(gen);
}