#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "lumina/lumina.hpp"
#include "../util.h"

#include <memory>
#include <string>

int main()
{
    const char* frame_title = "Interface";
    int frame_width = 1800;
    int frame_height = 1400;

    const int fps = 120;
    const int tps = 120;

    std::shared_ptr<lumina::Viewport> viewport_ptr = std::make_shared<lumina::Viewport>(frame_title, frame_width, frame_height);
    
    std::shared_ptr<lumina::Camera3D> camera_ptr = std::make_shared<lumina::Camera3D>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
    45.0f, (float)frame_width / frame_height, 0.1f, 100.0f);
    //std::shared_ptr<lumina::Camera2D> camera_ptr = std::make_shared<lumina::Camera2D>(frame_width, frame_height);
    
    std::shared_ptr<lumina::Shader> default_shader_ptr = std::make_shared<lumina::Shader>("../shaders/default_vertex_shader.glsl", "../shaders/default_fragment_shader.glsl");
    std::shared_ptr<lumina::Shader> font_shader_ptr = std::make_shared<lumina::Shader>("../external/lumina/shaders/font_vertex.glsl", "../external/lumina/shaders/font_fragment.glsl");

    std::shared_ptr<lumina::Font> font_ptr = std::make_shared<lumina::Font>("../external/lumina/fonts/roboto_mono/RobotoMono-SemiBold.ttf", font_shader_ptr);

    std::shared_ptr<lumina::Terminal> terminal_ptr = std::make_shared<lumina::Terminal>(viewport_ptr, font_ptr);

    std::vector<float> vertices;
    uint num_vertices;
    std::vector<uint> indices;
    uint num_indices;
    loadGeometryFromFile("../geometry/cube.txt", vertices, num_vertices, indices, num_indices);

    std::shared_ptr<std::vector<float>> vertices_ptr = std::make_shared<std::vector<float>>(vertices);
    std::shared_ptr<std::vector<unsigned int>> indices_ptr = std::make_shared<std::vector<unsigned int>>(indices);

    lumina::Mesh cube_mesh(vertices_ptr, num_vertices, indices_ptr, num_indices);
    std::shared_ptr<lumina::Mesh> cube_mesh_ptr = std::make_shared<lumina::Mesh>(cube_mesh);

    std::shared_ptr<lumina::Node> root_node_ptr = std::make_shared<lumina::Node>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
 
    std::shared_ptr<lumina::Node> piano_ptr = std::make_shared<lumina::Node>(glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);

    std::shared_ptr<lumina::Node> obstacle1_ptr = std::make_shared<lumina::Node>(glm::vec3(2.0f, 2.0f, 3.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    std::shared_ptr<lumina::Node> obstacle2_ptr = std::make_shared<lumina::Node>(glm::vec3(-2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    std::shared_ptr<lumina::Node> obstacle3_ptr = std::make_shared<lumina::Node>(glm::vec3(1.0f, 3.0f, -3.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    std::shared_ptr<lumina::Node> obstacle4_ptr = std::make_shared<lumina::Node>(glm::vec3(5.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    std::shared_ptr<lumina::Node> obstacle5_ptr = std::make_shared<lumina::Node>(glm::vec3(2.0f, -3.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    std::shared_ptr<lumina::Node> obstacle6_ptr = std::make_shared<lumina::Node>(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);

    root_node_ptr->addChild(piano_ptr);

    root_node_ptr->addChild(obstacle1_ptr);
    root_node_ptr->addChild(obstacle2_ptr);
    root_node_ptr->addChild(obstacle3_ptr);
    root_node_ptr->addChild(obstacle4_ptr);
    root_node_ptr->addChild(obstacle5_ptr);
    root_node_ptr->addChild(obstacle6_ptr);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_CULL_FACE);
    //glFrontFace(GL_CW);   // Winding order to clock-wise
    
    SDL_SetRelativeMouseMode(SDL_TRUE); // infinite mouse movement, no cursor, always in window

    // Pre-Loop Code
    SDL_Event event;
    bool running = true;

    uint64 sdl_ticks = SDL_GetTicks64();
    uint64 last_tick_time = sdl_ticks;
    uint64 last_update_time = sdl_ticks;
    uint64 last_render_time = sdl_ticks;
    uint64 last_time = sdl_ticks;
    uint64 current_time = sdl_ticks;

    float last_x = frame_width / 2.0f;
    float last_y = frame_height / 2.0f;
    bool first_mouse = true;

    while (running)
    {
        current_time = SDL_GetTicks64();

        uint64 delta_time = (current_time - last_time) / 1000.0f;
        uint64 last_time = current_time;

        while (SDL_PollEvent(&event))
        {
            if (terminal_ptr->selected)
            {
                terminal_ptr->processInput(event);
            }

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                float offset_x = event.motion.xrel;
                float offset_y = -event.motion.yrel;

                camera_ptr->processMouseMovement(offset_x, offset_y);
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                camera_ptr->processMouseScroll(event.wheel.y);
            }
            else if (event.type == SDL_KEYDOWN) 
            {
                SDL_Keycode key_pressed = event.key.keysym.sym;

                if (terminal_ptr->selected)
                {
                    switch (key_pressed)
                    {
                        case SDLK_ESCAPE:
                            terminal_ptr->deselect();
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    switch (key_pressed)
                    {
                        case SDLK_t:
                            terminal_ptr->select();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        float tick_delta_time = current_time - last_tick_time;
        float update_delta_time = current_time - last_update_time;
        float render_delta_time = current_time - last_render_time;

        if (tick_delta_time >= 1000 / tps)
        {
            if (!terminal_ptr->selected)
            {   
                camera_ptr->processKeyboard(tick_delta_time);
            }
            
            //root_node_ptr->rotation.y = root_node_ptr->rotation.y + 0.1f;
            //arm_node_ptr->rotation.z = arm_node_ptr->rotation.z + 0.1f;
            
            //base_node_ptr->setRotation(glm::vec3(base_node_ptr->rotation + glm::vec3(0.0f, 0.0f, 1.0f)));
            //glm::quat rotationChange = glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            //base_node_ptr->setRotation(glm::normalize(rotationChange * glm::quat(glm::vec3(base_node_ptr->rotation))));

            last_tick_time = current_time;
        }

        // UPDATE HERE
        root_node_ptr->update();

        last_update_time = current_time;

        if (render_delta_time >= 1000 / fps)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            root_node_ptr->render();

            terminal_ptr->render();

            font_ptr->shader_ptr->bind();
            glUniform4f(glGetUniformLocation(font_ptr->shader_ptr->shader_id, "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            int w, h;
            SDL_GetWindowSize(viewport_ptr->window, &w, &h);
            glm::mat4 ortho = glm::ortho(0.0f, (float)w, (float)h, 0.0f);
            glUniformMatrix4fv(glGetUniformLocation(font_ptr->shader_ptr->shader_id, "u_modelViewProj"), 1, GL_FALSE, glm::value_ptr(ortho));
            int x, y;
            int padding_top = 40;
            int padding_left = 10;
            lumina::relative2Pixel(0.0f, 0.0f, w, h, x, y);
            std::string debug_render_string = "";
            debug_render_string.append(std::to_string(camera_ptr->position.x));
            debug_render_string.append(" ");
            debug_render_string.append(std::to_string(camera_ptr->position.y));
            debug_render_string.append(" ");
            debug_render_string.append(std::to_string(camera_ptr->position.z));
            font_ptr->drawString(float(x+padding_left), float(y+padding_top), debug_render_string.c_str()); 
            font_ptr->shader_ptr->unbind();

            viewport_ptr->swapBuffers();

            // SDL_GL_SwapWindow(interface_viewport_ptr->window);
            last_render_time = current_time;
        }
    }
};