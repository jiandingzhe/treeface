#include "TestFramework.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/scene.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/scenenode.h"
#include "treeface/scene/visualobject.h"

#include "treeface/materialmanager.h"
#include "treeface/packagemanager.h"

#include <treejuce/File.h>
#include <treejuce/Result.h>
#include <treejuce/StringRef.h>

#define GLEW_STATIC
#include <GL/glew.h>
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
    pkg_mgr->add_package(File::getCurrentWorkingDirectory().getChildFile("../examples/resource.zip"), PackageManager::KEEP_EXISTING);

    Holder<Scene> scene = new Scene();
    OK(scene.get());

    Holder<MaterialManager> mat_mgr = scene->get_material_manager();
    Holder<GeometryManager> geo_mgr = scene->get_geometry_manager();

    OK(mat_mgr.get());
    OK(geo_mgr.get());

    // build scene
    OK(scene->build(String("scene.json")));

    OK(geo_mgr->geometry_is_cached("geom_colored.json"));
    OK(geo_mgr->geometry_is_cached("geom_cube.json"));

    Holder<Geometry> geo_colored = nullptr;
    Holder<Geometry> geo_cube = nullptr;
    OK(geo_mgr->get_geometry("geom_colored.json", geo_colored));
    OK(geo_mgr->get_geometry("geom_cube.json", geo_cube));

    OK(mat_mgr->material_is_cached("material1.json"));
    OK(mat_mgr->material_is_cached("material2.json"));
    OK(mat_mgr->material_is_cached("material_vert_color.json"));

    Material* mat1 = nullptr;
    Material* mat2 = nullptr;
    Material* mat_vert_color = nullptr;
    OK(mat_mgr->get_material("material1.json", &mat1));
    OK(mat_mgr->get_material("material2.json", &mat2));
    OK(mat_mgr->get_material("material_vert_color.json", &mat_vert_color));

    // scene properties
    const Vec4f& l_color = scene->get_global_light_color();
    IS_EPSILON(l_color.get_x(), 1);
    IS_EPSILON(l_color.get_y(), 0.5);
    IS_EPSILON(l_color.get_z(), 0.5);
    IS_EPSILON(l_color.get_w(), 1);

    const Vec4f& l_direction = scene->get_global_light_direction();
    IS_EPSILON(l_direction.get_x(), 0.707106781);
    IS_EPSILON(l_direction.get_y(), 0.707106781);
    IS_EPSILON(l_direction.get_z(), 0);
    IS_EPSILON(l_direction.get_w(), 0);

    const Vec4f& l_ambient = scene->get_global_light_ambient();
    IS_EPSILON(l_ambient.get_x(), 0.3);
    IS_EPSILON(l_ambient.get_y(), 0.2);
    IS_EPSILON(l_ambient.get_z(), 0.2);
    IS_EPSILON(l_ambient.get_w(), 1);

    // node hierarchy
    SceneNode* root_node = scene->get_root_node();
    SceneNode* node_a = scene->get_node("a");
    SceneNode* node_b = scene->get_node("b");
    SceneNode* node_c = scene->get_node("c");
    SceneNode* node_d = scene->get_node("d");
    OK(root_node);
    OK(node_a);
    OK(node_b);
    OK(node_c);
    OK(node_d);
    OK(root_node->has_child(node_a));
    OK(root_node->has_child(node_d));
    OK(node_a->has_child(node_b));
    OK(node_a->has_child(node_c));

    // node properties
    IS(node_a->get_num_items(), 0);
    IS(node_b->get_num_items(), 1);
    IS(node_c->get_num_items(), 1);
    IS(node_d->get_num_items(), 1);

    const Mat4f& trans_a = node_a->get_transform();
    {
        float val_00 = trans_a.get<0,0>();
        float val_10 = trans_a.get<1,0>();
        float val_20 = trans_a.get<2,0>();
        float val_30 = trans_a.get<3,0>();

        float val_01 = trans_a.get<0,1>();
        float val_11 = trans_a.get<1,1>();
        float val_21 = trans_a.get<2,1>();
        float val_31 = trans_a.get<3,1>();

        float val_02 = trans_a.get<0,2>();
        float val_12 = trans_a.get<1,2>();
        float val_22 = trans_a.get<2,2>();
        float val_32 = trans_a.get<3,2>();

        float val_03 = trans_a.get<0,3>();
        float val_13 = trans_a.get<1,3>();
        float val_23 = trans_a.get<2,3>();
        float val_33 = trans_a.get<3,3>();

        IS_EPSILON(val_00, 1);
        IS_EPSILON(val_10, 0);
        IS_EPSILON(val_20, 0);
        IS_EPSILON(val_30, 0);

        IS_EPSILON(val_01, 0);
        IS_EPSILON(val_11, 1);
        IS_EPSILON(val_21, 0);
        IS_EPSILON(val_31, 0);

        IS_EPSILON(val_02, 0);
        IS_EPSILON(val_12, 0);
        IS_EPSILON(val_22, 1);
        IS_EPSILON(val_32, 0);

        IS_EPSILON(val_03, 0.5);
        IS_EPSILON(val_13, 0.5);
        IS_EPSILON(val_23, 0.1);
        IS_EPSILON(val_33, 1);
    }

    VisualObject* item_b = dynamic_cast<VisualObject*>(node_b->get_item_at(0));
    VisualObject* item_c = dynamic_cast<VisualObject*>(node_c->get_item_at(0));
    VisualObject* item_d = dynamic_cast<VisualObject*>(node_d->get_item_at(0));

    OK(item_b);
    OK(item_c);
    OK(item_d);

    IS(item_b->get_geometry(), geo_colored.get());
    IS(item_c->get_geometry(), geo_cube.get());
    IS(item_d->get_geometry(), geo_colored.get());

    IS(item_b->get_material(), dynamic_cast<SceneGraphMaterial*>(mat_vert_color));
    IS(item_c->get_material(), dynamic_cast<SceneGraphMaterial*>(mat2));
    IS(item_d->get_material(), dynamic_cast<SceneGraphMaterial*>(mat1));

    SDL_GL_DeleteContext(context);
    SDL_Quit();
}
