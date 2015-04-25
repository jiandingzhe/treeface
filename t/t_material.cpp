#include "TestFramework.h"

#include "treeface/scene/material.h"

#include "treeface/imagemanager.h"
#include "treeface/packagemanager.h"
#include "treeface/programmanager.h"

#include <treejuce/File.h>
#include <treejuce/JSON.h>
#include <treejuce/Logger.h>
#include <treejuce/ScopedPointer.h>
#include <treejuce/Variant.h>
#include <treejuce/StringRef.h>

#include <GL/glew.h>

#include <cstdlib>

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
    ImageManager* img_mgr = ImageManager::getInstance();
    ProgramManager* prog_mgr = ProgramManager::getInstance();

    pkg_mgr->add_package(File("resource.zip"), PackageManager::KEEP_EXISTING);

    // material 1
    {
        ArrayRef<uint8> src_mat1 = pkg_mgr->get_item_data("material1.json");
        ScopedPointer<uint8> src_mat1_scope(src_mat1.get_data());
        OK(src_mat1.get_data());

        var root_node_mat1;
        OK(JSON::parse((char*)src_mat1.get_data(), root_node_mat1));

        Material* mat1 = new Material();
        OK(mat1->build(root_node_mat1));

        ok(prog_mgr->program_is_cached("common_vertex.glsl", "frag_one_tex.glsl"), "program common_vertex.glsl frag_one_tex.glsl is cached");
        Program* program_from_mgr = nullptr;
        OK(prog_mgr->get_program("common_vertex.glsl", "frag_one_tex.glsl", &program_from_mgr));
        IS(mat1->get_program(), program_from_mgr);

        IS(mat1->get_num_textures(), 1);
        OK(img_mgr->image_is_cached("moon.jpg"));

    }

    // material 2
    {
        ArrayRef<uint8> src_mat2 = pkg_mgr->get_item_data("material2.json");
        ScopedPointer<uint8> src_mat2_scope(src_mat2.get_data());
        OK(src_mat2.get_data());

        var root_node_mat2;
        OK(JSON::parse((char*)src_mat2.get_data(), root_node_mat2));

        Material* mat2 = new Material();
        OK(mat2->build(root_node_mat2));

        OK(prog_mgr->program_is_cached("common_vertex.glsl", "frag_two_tex.glsl"));

        IS(mat2->get_num_textures(), 2);
        OK(img_mgr->image_is_cached("moonbump.pfm"));
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();
}
