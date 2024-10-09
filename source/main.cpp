#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "lumina/lumina.hpp"
#include "../util.h"

#include "utils.hpp"

#include <memory>
#include <string>
#include <stack>

SDL_Window* createWindow(const char* title, int width, int height, int x, int y) {
    SDL_Window* window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    return window;
}

int main()
{
    const char* frame_title = "Interface";
    int frame_width = 1400;
    int frame_height = 1200;

    const char* slice_frame_title = "Slice";
    int slice_frame_width = 1200;
    int slice_frame_height = 1200;

    const int SLICE_GRID_SIZE_X = 256;
    const int SLICE_GRID_SIZE_Y = 256;
    const int SLICE_GRID_SIZE_Z = 256;

    const char* slice_compute_shader_source = "../shaders/slice_compute.glsl";
    const char* slice_vertex_shader_source = "../shaders/slice_vertex.glsl";
    const char* slice_fragment_shader_source = "../shaders/slice_fragment.glsl";

    const int fps = 120;
    const int tps = 120;

    SDL_Window* window = createWindow(frame_title, frame_width, frame_height, 0, 0);
    SDL_Window* slice_window = createWindow(slice_frame_title, slice_frame_width, slice_frame_height, 1400, 0);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GLContext slice_gl_context = SDL_GL_CreateContext(slice_window);

    SDL_GL_MakeCurrent(window, gl_context);

    glewExperimental = GL_TRUE;
    glewInit();

    // std::shared_ptr<lumina::Viewport> viewport_ptr = std::make_shared<lumina::Viewport>(frame_title, frame_width, frame_height);
    
    std::shared_ptr<lumina::Camera3D> camera_ptr = std::make_shared<lumina::Camera3D>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
    45.0f, (float)frame_width / frame_height, 0.1f, 100.0f);
    //std::shared_ptr<lumina::Camera2D> camera_ptr = std::make_shared<lumina::Camera2D>(frame_width, frame_height);
    
    std::shared_ptr<lumina::Shader> default_shader_ptr = std::make_shared<lumina::Shader>("../shaders/default_vertex_shader.glsl", "../shaders/default_fragment_shader.glsl");
    std::shared_ptr<lumina::Shader> font_shader_ptr = std::make_shared<lumina::Shader>("../external/lumina/shaders/font_vertex.glsl", "../external/lumina/shaders/font_fragment.glsl");

    std::shared_ptr<lumina::Font> font_ptr = std::make_shared<lumina::Font>("../external/lumina/fonts/roboto_mono/RobotoMono-SemiBold.ttf", font_shader_ptr);

    std::shared_ptr<lumina::Terminal> terminal_ptr = std::make_shared<lumina::Terminal>(window, font_ptr);

    std::vector<float> vertices;
    uint num_vertices;
    std::vector<uint> indices;
    uint num_indices;
    loadGeometryFromFile("../geometry/cuboid.txt", vertices, num_vertices, indices, num_indices);

    std::shared_ptr<std::vector<float>> vertices_ptr = std::make_shared<std::vector<float>>(vertices);
    std::shared_ptr<std::vector<unsigned int>> indices_ptr = std::make_shared<std::vector<unsigned int>>(indices);

    lumina::Mesh cuboid_mesh(vertices_ptr, num_vertices, indices_ptr, num_indices);
    std::shared_ptr<lumina::Mesh> cuboid_mesh_ptr = std::make_shared<lumina::Mesh>(cuboid_mesh);

    std::vector<float> quad_vertices;
    uint quad_num_vertices;
    std::vector<uint> quad_indices;
    uint quad_num_indices;
    loadGeometryFromFile("../geometry/quad.txt", quad_vertices, quad_num_vertices, quad_indices, quad_num_indices);

    std::shared_ptr<std::vector<float>> quad_vertices_ptr = std::make_shared<std::vector<float>>(quad_vertices);
    std::shared_ptr<std::vector<unsigned int>> quad_indices_ptr = std::make_shared<std::vector<unsigned int>>(quad_indices);

    lumina::Mesh quad_mesh(quad_vertices_ptr, quad_num_vertices, quad_indices_ptr, quad_num_indices);
    std::shared_ptr<lumina::Mesh> quad_mesh_ptr = std::make_shared<lumina::Mesh>(quad_mesh);

    std::vector<std::shared_ptr<lumina::Object>> objects;

    std::shared_ptr<lumina::Object> slice_plane = std::make_shared<lumina::Object>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), quad_mesh_ptr);

    std::shared_ptr<lumina::Object> obstacle_1 = std::make_shared<lumina::Object>(glm::vec3(-1.0f), glm::vec3(20.0f), glm::vec3(1.0f, 2.0f, 3.0f), cuboid_mesh_ptr);
    std::shared_ptr<lumina::Object> obstacle_2 = std::make_shared<lumina::Object>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(2.0f), cuboid_mesh_ptr);
    std::shared_ptr<lumina::Object> obstacle_3 = std::make_shared<lumina::Object>(glm::vec3(2.0f), glm::vec3(45.0f), glm::vec3(1.0f), cuboid_mesh_ptr);
    std::shared_ptr<lumina::Object> obstacle_4 = std::make_shared<lumina::Object>(glm::vec3(3.0f), glm::vec3(0.0f), glm::vec3(1.0f), cuboid_mesh_ptr);
    std::shared_ptr<lumina::Object> obstacle_5 = std::make_shared<lumina::Object>(glm::vec3(2.0f, 1.6f, 1.4f), glm::vec3(10.0f, 30.0f, 15.0f), glm::vec3(1.0f), cuboid_mesh_ptr);

    objects.push_back(obstacle_1);
    objects.push_back(obstacle_2);
    objects.push_back(obstacle_3);
    objects.push_back(obstacle_4);
    objects.push_back(obstacle_5);


    //std::shared_ptr<lumina::Node> root_node_ptr = std::make_shared<lumina::Node>(glm::vec3(2.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
 
    // std::shared_ptr<lumina::Node> piano_ptr = std::make_shared<lumina::Node>(glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);

    //std::shared_ptr<lumina::Node> obstacle1_ptr = std::make_shared<lumina::Node>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    //std::shared_ptr<lumina::Node> obstacle2_ptr = std::make_shared<lumina::Node>(glm::vec3(-2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    //std::shared_ptr<lumina::Node> obstacle3_ptr = std::make_shared<lumina::Node>(glm::vec3(1.0f, 3.0f, -3.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    //std::shared_ptr<lumina::Node> obstacle4_ptr = std::make_shared<lumina::Node>(glm::vec3(5.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    //std::shared_ptr<lumina::Node> obstacle5_ptr = std::make_shared<lumina::Node>(glm::vec3(2.0f, -3.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    //std::shared_ptr<lumina::Node> obstacle6_ptr = std::make_shared<lumina::Node>(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);

    // root_node_ptr->addChild(piano_ptr);

    //root_node_ptr->addChild(obstacle1_ptr);
    //root_node_ptr->addChild(obstacle2_ptr);
    //root_node_ptr->addChild(obstacle3_ptr);
    //root_node_ptr->addChild(obstacle4_ptr);
    //root_node_ptr->addChild(obstacle5_ptr);
    //root_node_ptr->addChild(obstacle6_ptr);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDisable(GL_CULL_FACE);
    //glFrontFace(GL_CW);   // Winding order to clock-wise
    
    SDL_SetRelativeMouseMode(SDL_TRUE); // infinite mouse movement, no cursor, always in window

    SDL_GL_MakeCurrent(slice_window, slice_gl_context);

    lumina::Shader slice_shader(slice_vertex_shader_source, slice_fragment_shader_source);
    GLuint compute_program = createComputeShaderProgram(slice_compute_shader_source);
    GLuint voxel_texture;
    setupVoxelTexture(voxel_texture, SLICE_GRID_SIZE_X, SLICE_GRID_SIZE_Y, SLICE_GRID_SIZE_Z);

    std::vector<glm::mat4> object_inverse_model_matrices;

    // TODO: Adjust shader and ssbo

    // Pre-Loop Code
    SDL_Event event;

    bool running = true;
    bool window1Open = true;
    bool window2Open = true;

    uint64 sdl_ticks = SDL_GetTicks64();
    uint64 last_tick_time = sdl_ticks;
    uint64 last_update_time = sdl_ticks;
    uint64 last_render_time = sdl_ticks;
    uint64 last_time = sdl_ticks;
    uint64 current_time = sdl_ticks;

    float last_x = frame_width / 2.0f;
    float last_y = frame_height / 2.0f;
    bool first_mouse = true;

    float slice_z = 0.0f;
    bool slice_rendered = false;

    glm::vec3 grid_origin = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 grid_scale = glm::vec3(8.0f, 8.0f, 8.0f);

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
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    if (event.window.windowID == SDL_GetWindowID(window)) {
                        window1Open = false;
                        SDL_DestroyWindow(window);
                    } 
                    else if (event.window.windowID == SDL_GetWindowID(slice_window)) {
                        window2Open = false;
                        SDL_DestroyWindow(slice_window);
                    }
                }
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

        // SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    if (event.window.windowID == SDL_GetWindowID(window)) {
                        window1Open = false;
                        SDL_DestroyWindow(window);
                    } 
                    else if (event.window.windowID == SDL_GetWindowID(slice_window)) {
                        window2Open = false;
                        SDL_DestroyWindow(slice_window);
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
        //root_node_ptr->update();

        slice_plane->position = grid_origin - (glm::vec3(0.0f, 0.0f, grid_scale.z) * (0.5f - slice_z));
        slice_plane->scale = grid_scale;

        last_update_time = current_time;

        if (render_delta_time >= 1000 / fps)
        {

            // Start of render block for main window

            SDL_GL_MakeCurrent(window, gl_context);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            //root_node_ptr->render();

            glm::mat4 view = camera_ptr->getViewMatrix();
            glm::mat4 projection = camera_ptr->getProjectionMatrix();

            default_shader_ptr->bind();

            for (auto current_object : objects)
            {
                
                glm::mat4 model = current_object->getModelMatrix();
                glUniformMatrix4fv(glGetUniformLocation(default_shader_ptr->shader_id, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(glGetUniformLocation(default_shader_ptr->shader_id, "view"), 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(default_shader_ptr->shader_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                glUniform4f(glGetUniformLocation(default_shader_ptr->shader_id, "color"), 0.9f, 0.3f, 0.4f, 1.0f);
                current_object->mesh->bind();
                glDrawElements(GL_TRIANGLES, current_object->mesh->getNumIndices(), GL_UNSIGNED_INT, 0);
                current_object->mesh->unbind();
                
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glm::mat4 model = slice_plane->getModelMatrix();
            glUniformMatrix4fv(glGetUniformLocation(default_shader_ptr->shader_id, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(default_shader_ptr->shader_id, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(default_shader_ptr->shader_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniform4f(glGetUniformLocation(default_shader_ptr->shader_id, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
            slice_plane->mesh->bind();
            glDrawElements(GL_TRIANGLES, slice_plane->mesh->getNumIndices(), GL_UNSIGNED_INT, 0);
            slice_plane->mesh->unbind();


            default_shader_ptr->unbind();

            terminal_ptr->render();

            font_ptr->shader_ptr->bind();


            glUniform4f(glGetUniformLocation(font_ptr->shader_ptr->shader_id, "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            int w, h;
            SDL_GetWindowSize(window, &w, &h);
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
            debug_render_string.append(" ");
            debug_render_string.append(std::to_string(slice_z));
            font_ptr->drawString(float(x+padding_left), float(y+padding_top), debug_render_string.c_str()); 
            font_ptr->shader_ptr->unbind();

            SDL_GL_SwapWindow(window);

            // Start of render block for slice window

            SDL_GL_MakeCurrent(slice_window, slice_gl_context);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Fill buffers

            for (auto current_object : objects)
            {
                object_inverse_model_matrices.push_back(glm::inverse(current_object->getModelMatrix()));
            }

            GLuint inverse_model_matrix_ssbo;
            glGenBuffers(1, &inverse_model_matrix_ssbo);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, inverse_model_matrix_ssbo);
            glBufferData(GL_SHADER_STORAGE_BUFFER, object_inverse_model_matrices.size() * sizeof(glm::mat4), object_inverse_model_matrices.data(), GL_STATIC_DRAW);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, inverse_model_matrix_ssbo);

            glUseProgram(compute_program);

            glUniform3i(glGetUniformLocation(compute_program, "grid_size"), SLICE_GRID_SIZE_X, SLICE_GRID_SIZE_Y, SLICE_GRID_SIZE_Z);
            glUniform3f(glGetUniformLocation(compute_program, "grid_origin"), grid_origin.x, grid_origin.y, grid_origin.z);
            glUniform3f(glGetUniformLocation(compute_program, "grid_scale"), grid_scale.x, grid_scale.y, grid_scale.z);
            glUniform3f(glGetUniformLocation(compute_program, "voxel_size"), 1.0f, 1.0f, 1.0f);
            glUniform1i(glGetUniformLocation(compute_program, "num_objects"), object_inverse_model_matrices.size());

            glDispatchCompute(SLICE_GRID_SIZE_X / 8, SLICE_GRID_SIZE_Y / 8, SLICE_GRID_SIZE_Z / 8);
            //glDispatchCompute(SLICE_GRID_SIZE_X, SLICE_GRID_SIZE_Y, SLICE_GRID_SIZE_Z);

            glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

            glUseProgram(0);

            GLuint VAO, VBO, EBO;
            setupQuad(VAO, VBO, EBO);

            slice_shader.bind();

            // Bind the voxel texture for the fragment shader
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_3D, voxel_texture);
            glUniform1i(glGetUniformLocation(slice_shader.shader_id, "voxel_texture"), 0);

            // Set the slice Z value
            glUniform1f(glGetUniformLocation(slice_shader.shader_id, "slice_z"), slice_z);

            // Render the quad (full screen slice)
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            slice_shader.unbind();

            SDL_GL_SwapWindow(slice_window);

            object_inverse_model_matrices.clear();

            //glDeleteBuffers(1, &position_ssbo); // Check if necessary
            //glDeleteBuffers(1, &rotation_ssbo); // Check if necessary
            //glDeleteBuffers(1, &scale_ssbo); // Check if necessary

            // Put it in update
            slice_z += 0.002f;
            if (slice_z > 1.0f)
                slice_z = 0.0f;

            last_render_time = current_time;
        }
    }
};