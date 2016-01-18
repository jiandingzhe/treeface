#include <SDL.h>
#include <stdio.h>

#ifdef main
#  undef main
#endif

void show_key_mod(Uint16 mask)
{
    printf("  modification masks: ");
    if (mask & KMOD_LSHIFT)
        printf("lshift ");
    if (mask & KMOD_RSHIFT)
        printf("rshift ");
    if (mask & KMOD_LCTRL)
        printf("lctrl ");
    if (mask & KMOD_RCTRL)
        printf("rctrl ");
    if (mask & KMOD_LALT)
        printf("lalt ");
    if (mask & KMOD_RALT)
        printf("ralt ");
    if (mask & KMOD_LGUI)
        printf("lwin ");
    if (mask & KMOD_RGUI)
        printf("rwin ");
    if (mask & KMOD_CAPS)
        printf("caps_lock ");
    if (mask & KMOD_NUM)
        printf("num_lock ");
    if (mask & KMOD_MODE)
        printf("altgr ");
    printf("\n");
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_TIMER & SDL_INIT_EVENTS);
    SDL_Window* window = SDL_CreateWindow("sdl keyboard scancode event test", 50, 50, 400, 400, SDL_WINDOW_OPENGL);

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
            case SDL_KEYDOWN:
                printf("key down: scan code %x, key code %x (%c)\n", event.key.keysym.scancode, event.key.keysym.sym, event.key.keysym.sym);
                show_key_mod(event.key.keysym.mod);
                break;
            case SDL_KEYUP:
                printf("key up: scan code %x, key code %x (%c)\n", event.key.keysym.scancode, event.key.keysym.sym, event.key.keysym.sym);
                show_key_mod(event.key.keysym.mod);
                break;
            }
        }

        if (should_exit) break;

        SDL_Delay(20);
    }

    return 0;
}

