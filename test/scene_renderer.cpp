#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/material.h"
#include "treeface/materialmanager.h"
#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/scene.h"
#include "treeface/scene/scenenode.h"
#include "treeface/scene/scenenodemanager.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/scenerenderer.h"

#include "treeface/gl/program.h"
#include "treeface/gl/texture.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexindexbuffer.h"

#include "treeface/math/mat4.h"
#include "treeface/math/quat.h"

#include <treejuce/File.h>
#include <treejuce/Holder.h>
#include <treejuce/JSON.h>
#include <treejuce/StringRef.h>
#include <treejuce/Variant.h>

using namespace treeface;
using namespace treejuce;

int window_w = 400;
int window_h = 400;
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

Holder<GeometryManager> geo_mgr;
Holder<MaterialManager> mat_mgr;
Holder<SceneNodeManager> node_mgr;
PackageManager* pkg_mgr = nullptr;

Holder<Scene> scene;

void build_up_gl()
{
    glEnable(GL_DEPTH_TEST);

    geo_mgr = new GeometryManager();
    mat_mgr = new MaterialManager();
    node_mgr = new SceneNodeManager(geo_mgr, mat_mgr);
    pkg_mgr = PackageManager::getInstance();


    File pkg_file = File::getCurrentWorkingDirectory().getChildFile("../examples/resource.zip");
    Logger::writeToLog("load package from file " + pkg_file.getFullPathName());
    pkg_mgr->add_package(pkg_file, PackageManager::USE_NEWER);

    // parse node hierarchy
    MemoryBlock node_hie_src;
    {
        Result re = pkg_mgr->get_item_data("nodes.json", node_hie_src);
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }

    var json_root_node;
    {
        Result re = JSON::parse(node_hie_src.toString(), json_root_node);
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }

    Holder<SceneNode> builded_nodes;
    {
        Result re = node_mgr->add_nodes(json_root_node, builded_nodes);
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }

    // create scene
    scene = new Scene();
    scene->get_root_node()->add_child(builded_nodes);
}


void main_loop(SDL_Window* window)
{
    Holder<SceneRenderer> renderer = new SceneRenderer();

    float angle = 0;

    while (1)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
            }

            if (event.type == SDL_QUIT)
            {
                should_exit = true;
            }
        }

        if (should_exit) break;

        renderer->render(Mat4f(), Mat4f(), scene);

        // finalize
        SDL_GL_SwapWindow(window);
        SDL_Delay(50);
    }
}

#undef main
int main(int argc, char** argv)
{
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl(&window, &context);
    build_up_gl();

    main_loop(window);

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}
