#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "lumina/lumina.hpp"
#include "../util.h"

#include <memory>

int main()
{
    const char* frame_title = "Interface";
    int frame_width = 2560;
    int frame_height = 1600;

    const int fps = 120;
    const int tps = 120;

    lumina::Viewport viewport(frame_title, frame_width, frame_height);
    
    std::shared_ptr<lumina::Camera3D> camera_ptr = std::make_shared<lumina::Camera3D>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
    45.0f, (float)frame_width / frame_height, 0.1f, 100.0f);
    
    //lumina::Shader default_shader("../shaders/default_vertex_shader.glsl", "../shaders/default_fragment_shader.glsl");
    std::shared_ptr<lumina::Shader> default_shader_ptr = std::make_shared<lumina::Shader>("../shaders/default_vertex_shader.glsl", "../shaders/default_fragment_shader.glsl");
    std::shared_ptr<lumina::Shader> font_shader_ptr = std::make_shared<lumina::Shader>("../external/lumina/shaders/font_vertex.glsl", "../external/lumina/shaders/font_fragment.glsl");

    std::vector<float> vertices;
    uint num_vertices;
    std::vector<uint> indices;
    uint num_indices;
    loadGeometryFromFile("../geometry/cube.txt", vertices, num_vertices, indices, num_indices);

    for (int i = 0; i < indices.size(); i++)
    {
        std::cout << indices[i] << std::endl;
    }

    std::cout << num_vertices << std::endl;
    std::cout << num_indices << std::endl;
        
    /*std::vector<float> vertices = {
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
        0, 2, 1,
        0, 3, 2,
        2, 7, 3,
        2, 6, 7,
        2, 5, 6,
        2, 5, 1,
        0, 4, 1,
        1, 5, 4,
        0, 4, 3,
        3, 7, 4,
        4, 6, 5,
        4, 7, 6
    };
    uint num_indices = 36;*/

    std::shared_ptr<std::vector<float>> vertices_ptr = std::make_shared<std::vector<float>>(vertices);
    std::shared_ptr<std::vector<unsigned int>> indices_ptr = std::make_shared<std::vector<unsigned int>>(indices);

    lumina::Mesh cube_mesh(vertices_ptr, num_vertices, indices_ptr, num_indices);
    std::shared_ptr<lumina::Mesh> cube_mesh_ptr = std::make_shared<lumina::Mesh>(cube_mesh);

    /*
    for (int i = 0; i < 10; i++)
    {
        float x, y, z;
        x = lumina::getRandomFloat(-10.0f, 10.0f);
        y = lumina::getRandomFloat(-10.0f, 10.0f);
        z = lumina::getRandomFloat(-10.0f, 10.0f);

        node_list.push_back(lumina::Node(glm::vec3(x, y, z), cube_mesh_ptr));
    }*/

    std::shared_ptr<lumina::Node> root_node_ptr = std::make_shared<lumina::Node>(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    std::shared_ptr<lumina::Node> arm_joint_ptr = std::make_shared<lumina::Node>(glm::vec3(0.0f, 0.75f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    std::shared_ptr<lumina::Node> arm_node_ptr = std::make_shared<lumina::Node>(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 2.0f, 0.5f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    std::shared_ptr<lumina::Node> hand_node_ptr = std::make_shared<lumina::Node>(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 1.0f), cube_mesh_ptr, default_shader_ptr, camera_ptr);
    
    root_node_ptr->addChild(arm_joint_ptr);
    arm_joint_ptr->addChild(arm_node_ptr);
    arm_node_ptr->addChild(hand_node_ptr);

    Font font;
    font.initFont("../external/lumina/fonts/roboto_mono/RobotoMono-SemiBold.ttf");

    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    //glFrontFace(GL_CW);   // Winding order to clock-wise
    
    SDL_SetRelativeMouseMode(SDL_TRUE); // infinite mouse movement, no cursor, always in window


    // Create EBO
    /*unsigned int EBO;
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
    */
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
        }

        float tick_delta_time = current_time - last_tick_time;
        float update_delta_time = current_time - last_update_time;
        float render_delta_time = current_time - last_render_time;

        if (tick_delta_time >= 1000 / tps)
        {
            camera_ptr->processKeyboard(tick_delta_time);

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

            /*default_shader.bind();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

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

            for (int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, node_list[i]->position);
                glUniformMatrix4fv(glGetUniformLocation(default_shader.shader_id, "model"), 1, GL_FALSE, glm::value_ptr(model));

                node_list[i]->mesh->bind();
                
                glDrawElements(GL_TRIANGLES, node_list[i]->mesh->getNumIndices(), GL_UNSIGNED_INT, 0);
                node_list[i]->mesh->unbind();
            }*/

            root_node_ptr->render();

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            font_shader_ptr->bind();
            glUniform4f(glGetUniformLocation(font_shader_ptr->shader_id, "u_color"), 0.5f, 0.5f, 0.5f, 1.0f);
            int w, h;
            SDL_GetWindowSize(viewport.window, &w, &h);
            glm::mat4 ortho = glm::ortho(0.0f, (float)w, (float)h, 0.0f);
            glUniformMatrix4fv(glGetUniformLocation(font_shader_ptr->shader_id, "u_modelViewProj"), 1, GL_FALSE, glm::value_ptr(ortho));
            font.drawString(100.0f, 100.0f, "Hello", font_shader_ptr); 
            font_shader_ptr->unbind();

            SDL_GL_SwapWindow(viewport.window);
            last_render_time = current_time;
        }
    }

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
};