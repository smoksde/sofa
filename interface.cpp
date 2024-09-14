#include "libs/glm/glm.hpp"
#include "libs/glm/gtx/norm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

#include "frame.hpp"
#include "defines.hpp"

int main()
{
    const char* frame_title = "Interface";
    int frame_width = 1400;
    int frame_height = 1400;

    const int fps = 1;
    const int tps = 1;

    Frame frame = Frame(frame_title, frame_width, frame_height);

    SDL_Event event;

    bool running = true;
    bool test_bool = true;

    uint64 sdl_ticks = SDL_GetTicks64();
    uint64 last_tick_time = sdl_ticks;
    uint64 last_update_time = sdl_ticks;
    uint64 last_render_time = sdl_ticks;
    uint64 current_time = sdl_ticks;

    while (running)
    {
        current_time = SDL_GetTicks64();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
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
            test_bool = !test_bool;
            if (test_bool)
            {
                glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            }
            else
            {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            }
            glClear(GL_COLOR_BUFFER_BIT);

            // DRAW OBJECTS HERE

            SDL_GL_SwapWindow(frame.window);
            last_render_time = current_time;
        }
    }
};