#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/material.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/materialmanager.h"

#include "treeface/gl/program.h"
#include "treeface/gl/texture.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexindexbuffer.h"

#include "treeface/math/mat4.h"
#include "treeface/math/quat.h"

#include <treejuce/File.h>
#include <treejuce/Holder.h>
#include <treejuce/StringRef.h>

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
PackageManager* pkg_mgr = nullptr;
Holder<VertexArray> vao_a = nullptr;
Holder<VertexArray> vao_b = nullptr;

String name_geo_a("geom_cube.json");
String name_geo_b("geom_colored.json");
String name_mat_a("material1.json");
String name_mat_b("material2.json");

Holder<Geometry> geo_a = nullptr;
Holder<Geometry> geo_b = nullptr;
Holder<SceneGraphMaterial> mat_a = nullptr;
Holder<SceneGraphMaterial> mat_b = nullptr;

void build_up_gl()
{
    glEnable(GL_DEPTH_TEST);

    geo_mgr = new GeometryManager();
    mat_mgr = new MaterialManager();
    pkg_mgr = PackageManager::getInstance();

    String pkg_file_name("../examples/resource.zip");
    Logger::writeToLog("load package from file " + pkg_file_name);
    pkg_mgr->add_package(File(pkg_file_name), PackageManager::USE_NEWER);

    {
        Result re_geo = geo_mgr->get_geometry(name_geo_a, geo_a);
        if (!re_geo)
        {
            die("failed to load geometry %s:\n"
                "%s\n",
                name_geo_a.toRawUTF8(),
                re_geo.getErrorMessage().toRawUTF8());
        }
    }

    {
        Result re = geo_mgr->get_geometry(name_geo_b, geo_b);
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }

    {
        Material* mat = nullptr;
        Result re_mat = mat_mgr->get_material(name_mat_a, &mat);
        if (!re_mat)
        {
            die("failed to load material %s:\n"
                "%s\n",
                name_mat_a.toRawUTF8(),
                re_mat.getErrorMessage().toRawUTF8());
        }
        mat_a = dynamic_cast<SceneGraphMaterial*>(mat);
    }

    {
        Material* mat = nullptr;
        Result re = mat_mgr->get_material(name_mat_b, &mat);
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
        mat_b = dynamic_cast<SceneGraphMaterial*>(mat);
    }

    // create vertex array, bind vertex attributes
    {
        vao_a = new VertexArray();
        Result re_vao = vao_a->build(geo_a->get_buffer(), geo_a->get_vertex_template(), mat_a->get_program());
        if (!re_vao)
        {
            die("failed to connect buffer and program with vertex array:\n"
                "%s\n",
                re_vao.getErrorMessage().toRawUTF8());
        }
    }

    {
        vao_b = new VertexArray();
        Result re = vao_b->build(geo_b->get_buffer(), geo_b->get_vertex_template(), mat_b->get_program());
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }
}


void main_loop(SDL_Window* window)
{
    Mat4f model_view_mat;
    Mat4f scale_3x;
    scale_3x.set_scale(3, 3, 3);

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

        angle += 0.02;
        Quatf rot(angle, Vec4f(1, 1, 0, 0));
        model_view_mat.set_rotate(rot);

        // draw A
        mat_a->use();
        mat_a->instant_set_transform(model_view_mat, Mat4f());

        vao_a->use();
        geo_a->get_buffer()->use();

        geo_a->get_buffer()->draw(geo_a->get_primitive());

        mat_a->unuse();
        vao_a->unuse();
        geo_a->get_buffer()->unuse();

        // draw B
        mat_b->use();
        mat_b->instant_set_transform(scale_3x, Mat4f());

        vao_b->use();
        geo_b->get_buffer()->use();

        geo_b->get_buffer()->draw(geo_b->get_primitive());

        mat_b->unuse();
        vao_b->unuse();
        geo_b->get_buffer()->unuse();

        // finalize
        SDL_GL_SwapWindow(window);
        SDL_Delay(50);
    }
}

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
