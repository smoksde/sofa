#include "libs/glm/glm.hpp"
#include "libs/glm/gtx/norm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

#include "frame.hpp"
#include "defines.hpp"
#include "util.h"
#include "shader.hpp"
#include "camera.hpp"

int main()
{
    const char* frame_title = "Interface";
    int frame_width = 1400;
    int frame_height = 1400;

    const int fps = 20;
    const int tps = 1;

    Frame frame = Frame(frame_title, frame_width, frame_height);
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
    45.0f, frame_width / frame_height, 0.1f, 100.0f);
    Shader default_shader = Shader("shaders/default_vertex_shader.glsl", "shaders/default_fragment_shader.glsl");

    // std::vector<float> vertices = loadVerticesFromFile("vertices/cube_vertices.txt");
    std::vector<float> vertices = {
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        -0.5, 0.5, -0.5,
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5
    };
    uint num_vertices = 8;

    std::vector<uint> indices =
    {
        0, 1, 2,
        0, 2, 3,
        2, 3, 7,
        2, 7, 6,
        2, 6, 5,
        2, 1, 5,
        0, 1, 4,
        1, 4, 5,
        0, 3, 4,
        3, 4, 7,
        4, 5, 6,
        4, 6, 7
    };
    uint num_indices = 36;

    std::vector<float>


    glEnable(GL_DEPTH_TEST);

    // Create EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

    // Create VAO and VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Pre-Loop Code
    SDL_Event event;
    bool running = true;

    uint64 sdl_ticks = SDL_GetTicks64();
    uint64 last_tick_time = sdl_ticks;
    uint64 last_update_time = sdl_ticks;
    uint64 last_render_time = sdl_ticks;
    uint64 current_time = sdl_ticks;

    float last_x = frame_width / 2.0f;
    float last_y = frame_height / 2.0f;
    bool first_mouse = true;

    while (running)
    {
        current_time = SDL_GetTicks64();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_w:
                        camera.processKeyboard("FORWARD", (current_time - last_tick_time) / 1000.0f);
                        break;
                    case SDLK_s:
                        camera.processKeyboard("BACKWARD", (current_time - last_tick_time) / 1000.0f);
                        break;
                    case SDLK_a:
                        camera.processKeyboard("LEFT", (current_time - last_tick_time) / 1000.0f);
                        break;
                    case SDLK_d:
                        camera.processKeyboard("RIGHT", (current_time - last_tick_time) / 1000.0f);
                        break;
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                // CHECK IF NECESSARY
                if (first_mouse)
                {
                    last_x = event.motion.x;
                    last_y = event.motion.y;
                    first_mouse = false;
                }

                float offset_x = event.motion.x - last_x;
                float offset_y = last_y - event.motion.y;
                last_x = event.motion.x;
                last_y = event.motion.y;

                camera.processMouseMovement(offset_x, offset_y);
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                camera.processMouseScroll(event.wheel.y);
            }
        }

        float tick_delta_time = current_time - last_tick_time;
        float update_delta_time = current_time - last_update_time;
        float render_delta_time = current_time - last_render_time;

        if (tick_delta_time >= 1000 / tps)
        {
            last_tick_time = current_time;
        }

        // UPDATE HERE

        last_update_time = current_time;

        if (render_delta_time >= 1000 / fps)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            default_shader.bind();

            glm::mat4 model = glm::mat4(1.0f); // Identity matrix (no transformation)
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translation
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotation
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Scaling

            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 projection = camera.getProjectionMatrix();

            glUniformMatrix4fv(glGetUniformLocation(default_shader.shader_id, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(default_shader.shader_id, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(default_shader.shader_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // DRAW OBJECTS HERE

            glBindVertexArray(VAO);
            // glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
            glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            SDL_GL_SwapWindow(frame.window);
            last_render_time = current_time;
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
};