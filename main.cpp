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

const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 800;

const char* read_shader_from_file(const std::string& filePath)
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

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create an SDL window and OpenGL context
    SDL_Window* window = SDL_CreateWindow("Sofa Problem - Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

    const char* vertexShaderSource = read_shader_from_file("shaders/vertex_shader.glsl");
    const char* fragmentShaderSource = read_shader_from_file("shaders/fragment_shader.glsl");

    // Compile and link the shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

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



    // Clean window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    unsigned int modelUniformLocation = glGetUniformLocation(shaderProgram, "model");
    unsigned int colorUniformLocation = glGetUniformLocation(shaderProgram, "u_color");
    glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(colorUniformLocation, 0.5f, 0.5f, 0.5f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 18);
   
    SDL_GL_SwapWindow(window);

    SDL_Delay(2000);

    return 0;
}