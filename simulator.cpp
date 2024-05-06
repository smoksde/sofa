#include "simulator.h"

Simulator::Simulator(int N, int frame_width, int frame_height) : N(N), frame_width(frame_width), frame_height(frame_height)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    }

    // Create an SDL window and OpenGL context
    window = SDL_CreateWindow("Sofa Problem - Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, frame_width, frame_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    // Create an OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // Initialize GLEW
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    vertexShaderSource = read_shader_from_file("shaders/simulator_vertex_shader.glsl");
    fragmentShaderSource = read_shader_from_file("shaders/loop_fragment_shader.glsl");

    // Compile and link the shaders
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    shaderProgram = glCreateProgram();
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
    glViewport(0, 0, frame_width, frame_height);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    uniform_N = glGetUniformLocation(shaderProgram, "N");
    uniform_location_angle = glGetUniformLocation(shaderProgram, "angle");
    uniform_location_offset_x = glGetUniformLocation(shaderProgram, "offsetX");
    uniform_location_offset_y = glGetUniformLocation(shaderProgram, "offsetY");
}

int Simulator::simulate(std::vector<float>& signal_yaw, std::vector<float>& signal_offset_x, std::vector<float>& signal_offset_y)
{
    // Clean window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform1i(uniform_N, N);
    glUniform1fv(uniform_location_angle, N, signal_yaw.data());
    glUniform1fv(uniform_location_offset_x, N, signal_offset_x.data());
    glUniform1fv(uniform_location_offset_y, N, signal_offset_y.data());

    glDrawArrays(GL_TRIANGLES, 0, 18);

    SDL_GL_SwapWindow(window);
    SDL_Delay(2000);

    int score = 0;
    return score;
}