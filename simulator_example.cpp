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
#include "fourier.h"

const int FRAME_WIDTH = 1400;
const int FRAME_HEIGHT = 1400;

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create an SDL window and OpenGL context
    SDL_Window* window = SDL_CreateWindow("Sofa Problem - Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create an OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize GLEW
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    const char* vertexShaderSource = read_shader_from_file("shaders/simulator_vertex_shader.glsl");
    const char* fragmentShaderSource = read_shader_from_file("shaders/loop_fragment_shader.glsl");

    // Compile and link the shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint vertexCompileStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompileStatus);
    if (vertexCompileStatus != GL_TRUE) {
        std::cerr << "Vertex shader compilation failed." << std::endl;
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(vertexShader, logLength, &logLength, &log[0]);
        std::cerr << "Vertex shader compile log:" << std::endl;
        std::cerr << log.data() << std::endl;
        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    GLint fragmentCompileStatus;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompileStatus);
    if (fragmentCompileStatus != GL_TRUE) {
        std::cerr << "Fragment shader compilation failed." << std::endl;
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(fragmentShader, logLength, &logLength, &log[0]);
        std::cerr << "Fragment shader compile log:" << std::endl;
        std::cerr << log.data() << std::endl;
        return -1;
    }
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        std::cerr << "Shader program linking failed." << std::endl;
        GLint logLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(shaderProgram, logLength, &logLength, &log[0]);
        std::cerr << "Shader program link log:" << std::endl;
        std::cerr << log.data() << std::endl;
        return -1;
    }

    float vertices[] = 
    {
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Set up OpenGL viewport
    glViewport(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    unsigned int uniform_N = glGetUniformLocation(shaderProgram, "N");
    unsigned int uniform_location_angle = glGetUniformLocation(shaderProgram, "angle");
    unsigned int uniform_location_offset_x = glGetUniformLocation(shaderProgram, "offsetX");
    unsigned int uniform_location_offset_y = glGetUniformLocation(shaderProgram, "offsetY");

    const int N = 10;

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

    for (int i = 1; i < N + 1; i++)
    {
        // Clean window
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1i(uniform_N, i);
        glUniform1fv(uniform_location_angle, i, signal_yaw.data());
        glUniform1fv(uniform_location_offset_x, i, signal_offset_x.data());
        glUniform1fv(uniform_location_offset_y, i, signal_offset_y.data());

        glDrawArrays(GL_TRIANGLES, 0, 18);

        SDL_GL_SwapWindow(window);
        SDL_Delay(400);
    }

    SDL_Delay(2000);

    return 0;
}