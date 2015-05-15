#include "TestFramework.h"
#include "treeface/common.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <cstdlib>

#include <SDL.h>

#include <treejuce/Logger.h>
#include <treejuce/StringRef.h>

using namespace std;
using namespace treeface;
using namespace treejuce;

int window_w = 400;
int window_h = 400;
void build_up_sdl(SDL_Window** window, SDL_GLContext* context)
{
    SDL_Init(SDL_INIT_VIDEO & SDL_INIT_TIMER & SDL_INIT_EVENTS);

    Logger::writeToLog("create window\n");
    *window = SDL_CreateWindow("sdl_setup test", 50, 50, window_w, window_h, SDL_WINDOW_OPENGL);
    if (!*window)
        die("error: failed to create window: %s\n", SDL_GetError());

    printf("create opengl context\n");
    *context = SDL_GL_CreateContext(*window);
    if (!context)
        die("error: failed to create GL context: %s\n", SDL_GetError());

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
    printf("glew inited\n");
}

void TestFramework::content()
{
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl(&window, &context);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
