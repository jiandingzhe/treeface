#include <SDL.h>
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>

int window_w = 200;
int window_h = 150;

#ifdef main
#  undef main
#endif


void build_up_sdl(SDL_Window** window, SDL_GLContext* context)
{
    SDL_Init(SDL_INIT_VIDEO & SDL_INIT_TIMER & SDL_INIT_EVENTS);

    printf("create window\n");
    *window = SDL_CreateWindow("sdl_setup test", 50, 50, window_w, window_h, SDL_WINDOW_OPENGL);
    if (!*window)
    {
        fprintf(stderr, "error: failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    printf("create opengl context\n");
    *context = SDL_GL_CreateContext(*window);
    if (!context)
    {
        fprintf(stderr, "error: failed to create GL context: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_MakeCurrent(*window, *context);

    printf("init glew\n");
    {
        GLenum glew_err = glewInit();
        if (glew_err != GLEW_OK)
        {
            fprintf(stderr, "error: failed to init glew: %s\n", glewGetErrorString(glew_err));
            exit(1);
        }
    }
}

void on_mouse_motion(SDL_MouseMotionEvent& e)
{

}

void on_mouse_down(SDL_MouseButtonEvent& e)
{
    printf("mouse down: button %d, combo %d\n", e.button, e.clicks);
}

void on_mouse_up(SDL_MouseButtonEvent& e)
{
    printf("mouse up: button %d, combo %d\n", e.button, e.clicks);
}

#undef main
int main(int argc, char** argv)
{
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl(&window, &context);

    while (1)
    {
        // process events
        SDL_Event event{};
        bool should_exit = false;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                should_exit = true;
                break;
            case SDL_MOUSEMOTION:
                on_mouse_motion(event.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
                on_mouse_down(event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                on_mouse_up(event.button);
                break;
            }

            if (event.type == SDL_QUIT)
            {
                should_exit = true;
            }
        }

        if (should_exit) break;

        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
        SDL_Delay(20);
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}
