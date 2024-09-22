#pragma once

#include <random>
#include <chrono>
#include <vector>

#include <sstream>
#include <fstream>
#include <cstring>

#include <iostream>
#include <string>

std::vector<float> loadVerticesFromFile(const std::string& filepath)
{
    std::vector<float> vertices;
    std::ifstream file(filepath);
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            float x, y, z;
            if (ss >> x >> y >> z)
            {
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filepath << std::endl;
    }

    return vertices;
}

inline const char* read_shader_from_file(const std::string& filePath)
{
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open())
    {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    std::string shaderSourceStr = shaderStream.str();
    char* shaderSource = new char[shaderSourceStr.length() + 1];
    strcpy(shaderSource, shaderSourceStr.c_str());

    return shaderSource;
}

inline double random_double(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distribution(min, max);

    return distribution(gen);
}

inline double random_float(float min, float max)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distribution(min, max);

    return distribution(gen);
}

inline int random_int(int min, int max) {
    // Use a high-resolution clock to seed the random number engine
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(min, max);

    // Generate a random number within the specified range
    return distribution(generator);
}

inline bool random_bool()
{
    // Use a high-resolution clock to seed the random number engine
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1); // 0 represents false, 1 represents true

    // Generate a random integer (0 or 1) and convert it to bool
    return static_cast<bool>(distribution(generator));
}

inline std::vector<float> linspace(float startValue, float endValue, int numValues) {
    std::vector<float> result(numValues);
    float step = (endValue - startValue) / float(numValues - 1);
    for (int i = 0; i < numValues; ++i) {
        result[i] = startValue + float(i) * step;
    }
    return result;
}