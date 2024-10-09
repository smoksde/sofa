#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "lumina/lumina.hpp"

#include <sstream>


inline GLuint createComputeShader(const char* shader_source)
{
    std::ifstream file(shader_source);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << shader_source << std::endl;
    }

    std::stringstream shader_stream;
    shader_stream << file.rdbuf();  // Read file content into the stream
    file.close();

    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    std::string shader_code = shader_stream.str();
    const char* shader_code_cstr = shader_code.c_str();
    glShaderSource(shader, 1, &shader_code_cstr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << info_log << std::endl;
        return 0;
    }

    return shader;
}

inline GLuint createComputeShaderProgram(const char* compute_shader_source)
{
    GLuint shader = createComputeShader(compute_shader_source);
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    glDeleteShader(shader);
    return program;
}

inline void setupVoxelTexture(GLuint &textureID, const int GRID_SIZE_X, const int GRID_SIZE_Y, const int GRID_SIZE_Z)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindImageTexture(0, textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

inline void setupBuffer(GLuint &ssbo, uint binding_point, const std::vector<glm::vec4> &objects, uint element_size)
{
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, objects.size() * element_size, objects.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, ssbo);
}

inline void setupQuad(GLuint &VAO, GLuint &VBO, GLuint &EBO) {
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f,   -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,   -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,   1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f
    };
    
    unsigned int quadIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}