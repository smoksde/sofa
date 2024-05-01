#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "matplotlibcpp.h"

#include "util.h"

namespace plt = matplotlibcpp;

double objective_function(double x, double y)
{
    return x*x + y*y;
}

int main()
{
    // Create data for contour plot
    std::vector<std::vector<double>> X, Y, Z;
    // Fill X, Y, Z with your data
    // Example:

    double lower_bound = -20;
    double upper_bound = 20;
    double step = 0.1;

    for (double x = lower_bound; x <= upper_bound; x += step) {
        std::vector<double> x_row, y_row, z_row;
        for (double y = lower_bound; y <= upper_bound; y += step) {
            x_row.push_back(x);
            y_row.push_back(y);
            z_row.push_back(objective_function(x,y));
        }
        X.push_back(x_row);
        Y.push_back(y_row);
        Z.push_back(z_row);
    }

    // Plot contour plot
    plt::contour(X, Y, Z);

    // Plot additional points
    std::vector<double> points_x;
    std::vector<double> points_y;


    std::vector<double> difference = {0.0, 0.0};

    // std::vector<double> start_point = {random_double(lower_bound, upper_bound), random_double(lower_bound, upper_bound)};
    std::vector<double> start_point = {-10, -10};
    int iterations = 40;
    int dimensions = 2;

    for (int i = 0; i < iterations; i++)
    {
        int try_count = 0;
        while (true)
        {
            std::vector<double> current_point = start_point;
            current_point[0] += random_double(-1.0, 1.0) + (random_double(0.4, 1.0) * difference[0]);// * std::abs(difference[0]) + difference[0];
            current_point[1] += random_double(-1.0, 1.0) + (random_double(0.4, 1.0) * difference[1]);// * std::abs(difference[0]) + difference[1];

            if (objective_function(current_point[0], current_point[1]) < objective_function(start_point[0], start_point[1]))
            {
                difference[0] = (0.6 * difference[0]) + (0.4 * (current_point[0] - start_point[0]));
                difference[1] = (0.6 * difference[1]) + (0.4 * (current_point[1] - start_point[1]));
                start_point = current_point;
                points_x.push_back(current_point[0]);
                points_y.push_back(current_point[1]);
                break;
            }
            try_count++;
            if (try_count >= 100)
                break;
        }
        
        
    }   

    plt::plot(points_x, points_y, "x");
    // plt::scatter(points_x, points_y, "x");

    // Draw lines connecting adjacent points
    for (size_t i = 0; i < points_x.size() - 1; ++i) {
        std::vector<double> line_x = {points_x[i], points_x[i + 1]};
        std::vector<double> line_y = {points_y[i], points_y[i + 1]};
        plt::plot(line_x, line_y, "-");
        std::cout << "x: " << points_x[i] << " y: " << points_y[i] << std::endl;
    }

    plt::show();

    return 0;
}