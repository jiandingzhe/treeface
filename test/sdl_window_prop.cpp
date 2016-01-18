#include <SDL.h>
#include <stdio.h>

#ifdef main
#  undef main
#endif

int window_w = 200;
int window_h = 150;

SDL_Window* window = nullptr;

void on_mouse_motion(SDL_MouseMotionEvent& e)
{

}

void on_mouse_down(SDL_MouseButtonEvent& e)
{
    //printf("mouse down %d %d: button %d, combo %d\n", e.x, e.y, e.button, e.clicks);
}

void on_mouse_up(SDL_MouseButtonEvent& e)
{
    //printf("mouse up %d %d: button %d, combo %d\n", e.x, e.y, e.button, e.clicks);
}

void on_key_down(SDL_KeyboardEvent& e)
{
    //printf("key down: scan code %d, key code %d\n", e.keysym.scancode, e.keysym.sym);
}

void on_key_up(SDL_KeyboardEvent& e)
{
    //printf("key up: scan code %d, key code %d\n", e.keysym.scancode, e.keysym.sym);
    switch (e.keysym.scancode)
    {
    case SDL_SCANCODE_F11:
        if (e.keysym.mod & KMOD_ALT)
            SDL_SetWindowFullscreen(window, 0);
        else if (e.keysym.mod & KMOD_CTRL)
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        else
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        break;
    case SDL_SCANCODE_B:
        if (e.keysym.mod & KMOD_ALT)
            SDL_SetWindowBordered(window, SDL_FALSE);
        else
            SDL_SetWindowBordered(window, SDL_TRUE);
        break;
    case SDL_SCANCODE_G:
        if (e.keysym.mod & KMOD_ALT)
            SDL_SetWindowGrab(window, SDL_FALSE);
        else
            SDL_SetWindowGrab(window, SDL_TRUE);
        break;
    }
}

Uint32 on_time_out(Uint32 dt, void*)
{
    Uint32 flags = SDL_GetWindowFlags(window);

    printf("#\n");
    printf("full screen  : %d\n", bool(flags & SDL_WINDOW_FULLSCREEN));
    printf("full screen d: %d\n", (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP );
    printf("OpenGL       : %d\n", bool(flags & SDL_WINDOW_OPENGL));
    printf("shown        : %d\n", bool(flags & SDL_WINDOW_SHOWN));
    printf("hidden       : %d\n", bool(flags & SDL_WINDOW_HIDDEN));
    printf("no border    : %d\n", bool(flags & SDL_WINDOW_BORDERLESS));
    printf("resizable    : %d\n", bool(flags & SDL_WINDOW_RESIZABLE));
    printf("minimized    : %d\n", bool(flags & SDL_WINDOW_MINIMIZED));
    printf("maximized    : %d\n", bool(flags & SDL_WINDOW_MAXIMIZED));
    printf("input grabbed: %d\n", bool(flags & SDL_WINDOW_INPUT_GRABBED));
    printf("input focus  : %d\n", bool(flags & SDL_WINDOW_INPUT_FOCUS));
    printf("mouse focus  : %d\n", bool(flags & SDL_WINDOW_MOUSE_FOCUS));
    printf("foreign      : %d\n", bool(flags & SDL_WINDOW_FOREIGN));
    printf("high DPI     : %d\n", bool(flags & SDL_WINDOW_ALLOW_HIGHDPI));
    printf("#\n");

    return dt;
}

void build_up_sdl()
{
    SDL_Init(SDL_INIT_VIDEO & SDL_INIT_TIMER & SDL_INIT_EVENTS);

    printf("create window\n");
    window = SDL_CreateWindow("sdl_setup test", 50, 50, window_w, window_h, SDL_WINDOW_OPENGL);
    if (!window)
    {
        fprintf(stderr, "error: failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_AddTimer(300, on_time_out, nullptr);
}


#undef main
int main(int argc, char** argv)
{
    build_up_sdl();

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
            case SDL_KEYDOWN:
                on_key_down(event.key);
                break;
            case SDL_KEYUP:
                on_key_up(event.key);
                break;
            }

            if (event.type == SDL_QUIT)
            {
                should_exit = true;
            }
        }

        if (should_exit) break;
    }

    SDL_Quit();

    return 0;
}
