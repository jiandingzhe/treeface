#include <SDL.h>

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
    SDL_Window* window = SDL_CreateWindow("sdl text test", 50, 50, 400, 400, SDL_WINDOW_OPENGL);

    SDL_StartTextInput();

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
            case SDL_TEXTEDITING:
                printf("text edit: %s %d %d\n", event.edit.text, event.edit.start, event.edit.length);
                break;
            case SDL_TEXTINPUT:
                printf("text input: %s\n", event.text.text);
                break;
            }
        }

        if (should_exit) break;

        SDL_Delay(20);
    }

    SDL_StopTextInput();
}

