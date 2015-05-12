#include "TestFramework.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/scene/geometrymanager.h"
#include "treeface/packagemanager.h"

#include "treeface/gl/vertexindexbuffer.h"

#include "treeface/scene/geometry.h"

#include <treejuce/File.h>
#include <treejuce/StringRef.h>
#include <treejuce/MemoryBlock.h>

#include <SDL.h>

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
}

void TestFramework::content()
{
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl(&window, &context);

    PackageManager* pkg_mgr = PackageManager::getInstance();
    Holder<GeometryManager> geom_mgr = new GeometryManager();

    pkg_mgr->add_package(File("../examples/resource.zip"), PackageManager::KEEP_EXISTING);

    Geometry* geom = nullptr;
    OK(geom_mgr->get_geometry("geom_colored.json", &geom));

    IS(geom->get_primitive(), GL_TRIANGLES);

    VertexIndexBuffer* vtx_idx_buf = geom->get_buffer();
    MemoryBlock& data_vtx = vtx_idx_buf->m_data_vtx;
    GLbyte* data_p = (GLbyte*) data_vtx.getData();

    IS(data_vtx.getSize(), 96);
    IS(*(float*)(data_p + 0), 0);
    IS(*(float*)(data_p + 4), 0);
    IS(*(float*)(data_p + 8), 0);

    IS(*(float*)(data_p + 12), 0);
    IS(*(float*)(data_p + 16), 0);

    IS(*(GLubyte*)(data_p + 20), 127);
    IS(*(GLubyte*)(data_p + 21), 127);
    IS(*(GLubyte*)(data_p + 22), 127);
    IS(*(GLubyte*)(data_p + 23), 255);

    IS(*(float*)(data_p + 24), 0.5);
    IS(*(float*)(data_p + 28), 0);
    IS(*(float*)(data_p + 32), 0);

    IS(*(float*)(data_p + 36), 0);
    IS(*(float*)(data_p + 40), 1);

    IS(*(GLubyte*)(data_p + 44), 255);
    IS(*(GLubyte*)(data_p + 45), 0);
    IS(*(GLubyte*)(data_p + 46), 0);
    IS(*(GLubyte*)(data_p + 47), 255);

    IS(*(float*)(data_p + 48), 0);
    IS(*(float*)(data_p + 52), 0.5);
    IS(*(float*)(data_p + 56), 0);

    IS(*(float*)(data_p + 60), 1);
    IS(*(float*)(data_p + 64), 0);

    IS(*(GLubyte*)(data_p + 68), 0);
    IS(*(GLubyte*)(data_p + 69), 255);
    IS(*(GLubyte*)(data_p + 70), 0);
    IS(*(GLubyte*)(data_p + 71), 255);

    IS(*(float*)(data_p + 72), 0);
    IS(*(float*)(data_p + 76), 0);
    IS(*(float*)(data_p + 80), 0.5);

    IS(*(float*)(data_p + 84), 1);
    IS(*(float*)(data_p + 88), 1);

    IS(*(GLubyte*)(data_p + 92), 0);
    IS(*(GLubyte*)(data_p + 93), 0);
    IS(*(GLubyte*)(data_p + 94), 255);
    IS(*(GLubyte*)(data_p + 95), 255);

    Array<uint16>& data_idx = vtx_idx_buf->m_data_idx;
    IS(data_idx.size(), 12);
    IS(data_idx[0], 0);
    IS(data_idx[1], 2);
    IS(data_idx[2], 1);
    IS(data_idx[3], 0);
    IS(data_idx[4], 1);
    IS(data_idx[5], 3);
    IS(data_idx[6], 0);
    IS(data_idx[7], 3);
    IS(data_idx[8], 2);
    IS(data_idx[9], 1);
    IS(data_idx[10], 2);
    IS(data_idx[11], 3);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
}
