#include <vector>
#include <iostream>

#include "simulator.h"

const int FRAME_WIDTH = 1400;
const int FRAME_HEIGHT = 1400;
const int N = 100;

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

    for (int i = 0; i < 10; i++)
    {
        simulator.simulate(signal_yaw, signal_offset_x, signal_offset_y);
    }
}