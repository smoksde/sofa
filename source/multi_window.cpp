#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <cmath>

void initSDLAndGLEW();
SDL_Window* createWindow(const char* title, int width, int height, int x, int y);
void renderWindow(SDL_Window* window, SDL_GLContext& glContext, float r, float g, float b);
void handleEvents(bool& running, bool& window1Open, bool& window2Open, SDL_Window* window1, SDL_Window* window2);

int main(int argc, char* argv[]) {
    initSDLAndGLEW();

    SDL_Window* window1 = createWindow("Window 1", 800, 600, 100, 100);
    SDL_Window* window2 = createWindow("Window 2", 800, 600, 920, 100);

    SDL_GLContext glContext1 = SDL_GL_CreateContext(window1);
    SDL_GLContext glContext2 = SDL_GL_CreateContext(window2);

    glewExperimental = GL_TRUE;
    glewInit();

    // Run loop
    bool running = true;
    bool window1Open = true;
    bool window2Open = true;

    float x = 0.0f;
    
    while (running) {
        handleEvents(running, window1Open, window2Open, window1, window2);

        if (window1Open) {
            renderWindow(window1, glContext1, sin(x) * 0.5 + 0.5, 0.0f, 0.0f);
        }

        if (window2Open) {
            renderWindow(window2, glContext2, 0.0f, cos(x) * 0.5 + 0.5, 0.0f);
        }

        if (!window1Open && !window2Open) {
            running = false;
        }

        SDL_Delay(16);
        x += 0.01f;
    }

    SDL_GL_DeleteContext(glContext1);
    SDL_GL_DeleteContext(glContext2);
    SDL_DestroyWindow(window1);
    SDL_DestroyWindow(window2);
    SDL_Quit();

    return 0;
}

void initSDLAndGLEW() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

SDL_Window* createWindow(const char* title, int width, int height, int x, int y) {
    SDL_Window* window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    return window;
}

void renderWindow(SDL_Window* window, SDL_GLContext& glContext, float r, float g, float b) {
    SDL_GL_MakeCurrent(window, glContext);
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
}

void handleEvents(bool& running, bool& window1Open, bool& window2Open, SDL_Window* window1, SDL_Window* window2) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                if (event.window.windowID == SDL_GetWindowID(window1)) {
                    window1Open = false;
                    SDL_DestroyWindow(window1);
                } 
                else if (event.window.windowID == SDL_GetWindowID(window2)) {
                    window2Open = false;
                    SDL_DestroyWindow(window2);
                }
            }
        }
    }
}