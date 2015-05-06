#include "TestFramework.h"

#include "treeface/imagemanager.h"

#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/material.h"
#include "treeface/scene/materialmanager.h"
#include "treeface/scene/node.h"
#include "treeface/scene/nodemanager.h"
#include "treeface/scene/visualitem.h"

#include "treeface/packagemanager.h"

#include "treeface/gl/programmanager.h"

#include <treejuce/File.h>
#include <treejuce/Holder.h>
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

    Holder<ProgramManager> prog_mgr = new ProgramManager();
    Holder<MaterialManager> mat_mgr = new MaterialManager(prog_mgr);
    Holder<GeometryManager> geo_mgr = new GeometryManager();
    Holder<NodeManager> node_mgr = new NodeManager(geo_mgr, mat_mgr);

    pkg_mgr->add_package(File("./resource.zip"), PackageManager::KEEP_EXISTING);

    OK(node_mgr->add_nodes(String("nodes.json")));
    Node* node_a = node_mgr->get_node("a");
    Node* node_b = node_mgr->get_node("b");
    OK(node_a != nullptr);
    OK(node_b != nullptr);

    OK(mat_mgr->material_is_cached("material_plain.json"));
    OK(mat_mgr->material_is_cached("material1.json"));
    OK(mat_mgr->material_is_cached("material_vert_color.json"));

    OK(geo_mgr->geometry_is_cached("geom_cube.json"));
    OK(geo_mgr->geometry_is_cached("geom_colored.json"));
    OK(geo_mgr->geometry_is_cached("geom_simple.json"));

    Material* material_plain = nullptr;
    Material* material1 = nullptr;
    Material* material_vert_color = nullptr;

    OK(mat_mgr->get_material("material_plain.json", &material_plain));
    OK(mat_mgr->get_material("material1.json", &material1));
    OK(mat_mgr->get_material("material_vert_color.json", &material_vert_color));

    Geometry* geom_cube = nullptr;
    Geometry* geom_colored = nullptr;
    Geometry* geom_simple = nullptr;

    OK(geo_mgr->get_geometry("geom_cube.json", &geom_cube));
    OK(geo_mgr->get_geometry("geom_colored.json", &geom_colored));
    OK(geo_mgr->get_geometry("geom_simple.json", &geom_simple));

    // node A children
    IS(node_a->get_num_children(), 2);

    Node* unamed_node = nullptr;
    bool b_got = false;
    for (int i = 0; i < node_a->get_num_children(); i++){
        Node* tmp = node_a->get_child_at(i);
        if (tmp == node_b)
            b_got = true;
        else
            unamed_node = tmp;
    }

    OK(b_got);
    OK(unamed_node);

    // node A visual item
    IS(node_a->get_num_visual_items(), 1);

    VisualItem* visual_a = node_a->get_visual_item_at(0);
    IS(visual_a->get_geometry(), geom_colored);
    IS(visual_a->get_material(), material_vert_color);

    // contents of unamed node
    IS(unamed_node->get_num_children(), 0);
    IS(unamed_node->get_num_visual_items(), 1);

    VisualItem* visual_unamed = unamed_node->get_visual_item_at(0);
    IS(visual_unamed->get_geometry(), geom_simple);
    IS(visual_unamed->get_material(), material_plain);

    // contents of node B
    IS(node_b->get_num_children(), 0);
    IS(node_b->get_num_visual_items(), 2);

    VisualItem* visual_b1 = node_b->get_visual_item_at(0);
    IS(visual_b1->get_geometry(), geom_cube);
    IS(visual_b1->get_material(), material_plain);

    VisualItem* visual_b2 = node_b->get_visual_item_at(1);
    IS(visual_b2->get_geometry(), geom_colored);
    IS(visual_b2->get_material(), material1);

    SDL_GL_DeleteContext(context);
    SDL_Quit();
}
