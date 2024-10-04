#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "lumina/lumina.hpp"

#include <sstream>
#include <iostream>
#include <vector>

const int GRID_SIZE = 128;

const int GRID_SIZE_X = GRID_SIZE;
const int GRID_SIZE_Y = GRID_SIZE;
const int GRID_SIZE_Z = GRID_SIZE;

const char* compute_shader_source = "../shaders/slice_compute.glsl";

const char* vertex_shader_source = "../shaders/slice_vertex.glsl";
const char* fragment_shader_source = "../shaders/slice_fragment.glsl";

GLuint createComputeShader(const char* shader_source)
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

GLuint createShaderProgram()
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

void setupVoxelTexture(GLuint &textureID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindImageTexture(0, textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void setupObjectBuffer(GLuint &ssbo, const std::vector<glm::vec4> &objects)
{
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, objects.size() * sizeof(glm::vec4), objects.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);
}

// Setup the full screen quad VAO/VBO for rendering
void setupQuad(GLuint &VAO, GLuint &VBO, GLuint &EBO) {
    float quadVertices[] = {
        // Positions       // Texture coordinates
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

int main(int argc, char* argv[])
{
    
    const char* frame_title = "Slice";
    int frame_width = 1800;
    int frame_height = 1400;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);  // OpenGL 4.x
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // After creating the OpenGL context
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    std::shared_ptr<lumina::Viewport> viewport_ptr = std::make_shared<lumina::Viewport>(frame_title, frame_width, frame_height);

    lumina::Shader shader(vertex_shader_source, fragment_shader_source);
    GLuint compute_program = createShaderProgram();
    GLuint voxel_texture;
    setupVoxelTexture(voxel_texture);

    std::vector<glm::vec4> objects = {
        glm::vec4(0.0f, 0.0f, 0.0f, 100.0f)
        // glm::vec4(32.0f, 32.0f, 32.0f, 20.0f),
        // glm::vec4(64.0f, 64.0f, 64.0f, 15.0f),
    };

    GLuint ssbo;
    setupObjectBuffer(ssbo, objects);
    glUseProgram(compute_program);

    glUniform3i(glGetUniformLocation(compute_program, "grid_size"), GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
    glUniform3f(glGetUniformLocation(compute_program, "grid_origin"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(compute_program, "voxel_size"), 1.0f, 1.0f, 1.0f);

    glDispatchCompute(GRID_SIZE_X / 8, GRID_SIZE_Y / 8, GRID_SIZE_Z / 8);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    GLuint VAO, VBO, EBO;
    setupQuad(VAO, VBO, EBO);

    bool running = true;
    float slice_z = 0.0f;  // Start in the middle

    while (running) {
        // Handle SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // Use the slice shader program
        shader.bind();

        // Bind the voxel texture for the fragment shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, voxel_texture);
        glUniform1i(glGetUniformLocation(shader.shader_id, "voxel_texture"), 0);

        // Set the slice Z value
        glUniform1f(glGetUniformLocation(shader.shader_id, "slice_z"), slice_z);

        // Render the quad (full screen slice)
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        shader.unbind();

        slice_z += 0.005;

        // Swap buffers (display the rendered frame)
        viewport_ptr->swapBuffers();
    }

    //std::vector<float> voxel_data(GRID_SIZE_X * GRID_SIZE_Y * GRID_SIZE_Z * 4);
    //glBindTexture(GL_TEXTURE_3D, voxel_texture);
    //glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_FLOAT, voxel_data.data());

    glDeleteProgram(compute_program);
    glDeleteBuffers(1, &ssbo);
    glDeleteTextures(1, &voxel_texture);

    viewport_ptr->~Viewport();
}