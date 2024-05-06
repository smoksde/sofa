#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <sstream>
#include <fstream>
#include <cstring>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

#include "util.h"

class Simulator
{
    public:

    int N;
    int frame_width;
    int frame_height;

    SDL_Window* window;
    SDL_GLContext glContext;

    const char* vertexShaderSource;
    const char* fragmentShaderSource;

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    
    unsigned int uniform_N;
    unsigned int uniform_location_angle;
    unsigned int uniform_location_offset_x;
    unsigned int uniform_location_offset_y;

    Simulator(int N, int frame_width, int frame_height);
    int simulate(std::vector<float>& signal_yaw, std::vector<float>& signal_offset_x, std::vector<float>& signal_offset_y);
};