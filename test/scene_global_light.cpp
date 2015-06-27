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
#include "treeface/scene/visualobject.h"

#include "treeface/gl/program.h"
#include "treeface/gl/texture.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexindexbuffer.h"

#include "treeface/math/mat4.h"
#include "treeface/math/quat.h"

#include <treejuce/File.h>
#include <treejuce/Holder.h>
#include <treejuce/JSON.h>
#include <treejuce/Result.h>
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

    // create scene
    scene = new Scene(geo_mgr, mat_mgr);
    scene->set_global_light_ambient(0.2, 0.2, 0.2, 1);

    Holder<Geometry> geom;
    {
        Result re = geo_mgr->get_geometry("geom_color_norm.json", geom);
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }

    Holder<Material> mat;
    {
        Result re = mat_mgr->get_material("material_color_light.json", mat);
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }

    VisualObject* item = new VisualObject();
    {
        Result re = item->build(geom, dynamic_cast<SceneGraphMaterial*>(mat.get()));
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }
    SceneNode* node = new SceneNode();
    node->add_item(item);
    scene->get_root_node()->add_child(node);
}


void main_loop(SDL_Window* window)
{
    SceneNode* node = scene->get_root_node()->get_child_at(0);

    Holder<SceneRenderer> renderer = new SceneRenderer();

    float angle = 0;
    Mat4f mat;

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

        mat.set_rotate(Quatf(angle, Vec4f(1,-1,0,0)));
        node->set_transform(mat);
        renderer->render(Mat4f(), Mat4f(), scene);

        // finalize
        SDL_GL_SwapWindow(window);
        SDL_Delay(50);
        angle += 0.1;
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
