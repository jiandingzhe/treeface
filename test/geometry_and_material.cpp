#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/material.h"
#include "treeface/scene/materialmanager.h"

#include "treeface/gl/program.h"
#include "treeface/gl/programmanager.h"
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
Holder<ProgramManager> prg_mgr;
Holder<MaterialManager> mat_mgr;
PackageManager* pkg_mgr = nullptr;
VertexArray* vao = nullptr;

String name_geo("geom_cube.json");
String name_mat("material1.json");

Holder<Geometry> geo = nullptr;
Holder<Material> mat = nullptr;

void build_up_gl()
{
    glEnable(GL_DEPTH_TEST);

    geo_mgr = new GeometryManager();
    prg_mgr = new ProgramManager();
    mat_mgr = new MaterialManager(prg_mgr);
    pkg_mgr = PackageManager::getInstance();

    String pkg_file_name("./resource.zip");
    Logger::writeToLog("load package from file " + pkg_file_name);
    pkg_mgr->add_package(File(pkg_file_name), PackageManager::USE_NEWER);

    {
        Result re_geo = geo_mgr->get_geometry(name_geo, geo);
        if (!re_geo)
        {
            die("failed to load geometry %s:\n"
                "%s\n",
                name_geo.toRawUTF8(),
                re_geo.getErrorMessage().toRawUTF8());
        }
    }

    {
        Result re_mat = mat_mgr->get_material(name_mat, mat);
        if (!re_mat)
        {
            die("failed to load material %s:\n"
                "%s\n",
                name_mat.toRawUTF8(),
                re_mat.getErrorMessage().toRawUTF8());
        }
    }

    {
        // create vertex array, bind vertex attributes
        vao = new VertexArray();
        Result re_vao = vao->build(geo->get_buffer(), geo->get_vertex_template(), mat->get_program());
        if (!re_vao)
        {
            die("failed to connect buffer and program with vertex array:\n"
                "%s\n",
                re_vao.getErrorMessage().toRawUTF8());
        }
    }
}


void main_loop(SDL_Window* window)
{
    GLint idx_uni_mat = mat->get_program()->get_uniform_index("matrix_model_view");
    Mat4f model_view_mat;

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

        mat->use();
        mat->get_program()->instant_set_uniform(idx_uni_mat, model_view_mat);

        vao->use();
        geo->get_buffer()->use();

        geo->get_buffer()->draw(geo->get_primitive());

        mat->unuse();
        vao->unuse();
        geo->get_buffer()->unuse();

        SDL_GL_SwapWindow(window);
        SDL_Delay(20);
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
