#include "TestFramework.h"

#include "treeface/imagemanager.h"
#include "treeface/materialmanager.h"
#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/scenenode.h"
#include "treeface/scene/scenenodemanager.h"
#include "treeface/scene/visualobject.h"

#include <treecore/File.h>
#include <treecore/RefCountHolder.h>
#include <treecore/JSON.h>
#include <treecore/Logger.h>
#include <treecore/ScopedPointer.h>
#include <treecore/Variant.h>
#include <treecore/StringRef.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <cstdlib>

#include <SDL.h>

using namespace treeface;
using namespace treecore;

int window_w = 400;
int window_h = 400;
void build_up_sdl( SDL_Window** window, SDL_GLContext* context )
{
    SDL_Init( SDL_INIT_VIDEO & SDL_INIT_TIMER & SDL_INIT_EVENTS );

    Logger::writeToLog( "create window\n" );
    *window = SDL_CreateWindow( "sdl_setup test", 50, 50, window_w, window_h, SDL_WINDOW_OPENGL );
    if (!*window)
        die( "error: failed to create window: %s\n", SDL_GetError() );

    printf( "create opengl context\n" );
    *context = SDL_GL_CreateContext( *window );
    if (!context)
        die( "error: failed to create GL context: %s\n", SDL_GetError() );

    SDL_GL_MakeCurrent( *window, *context );

    printf( "init glew\n" );
    {
        GLenum glew_err = glewInit();
        if (glew_err != GLEW_OK)
        {
            fprintf( stderr, "error: failed to init glew: %s\n", glewGetErrorString( glew_err ) );
            exit( 1 );
        }
    }
}

void TestFramework::content()
{
    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl( &window, &context );

    PackageManager* pkg_mgr = PackageManager::getInstance();

    RefCountHolder<MaterialManager>  mat_mgr  = new MaterialManager();
    RefCountHolder<GeometryManager>  geo_mgr  = new GeometryManager();
    RefCountHolder<SceneNodeManager> node_mgr = new SceneNodeManager( geo_mgr, mat_mgr );

    pkg_mgr->add_package( File::getCurrentWorkingDirectory().getChildFile( "../examples/resource.zip" ), PackageManager::KEEP_EXISTING );

    RefCountHolder<SceneNode> root_node;
    {
        var root_data = pkg_mgr->get_item_json( "nodes.json" );
        OK( root_data );
        root_node = node_mgr->add_nodes( root_data );
    }
    OK( root_node != nullptr );

    SceneNode* node_a = node_mgr->get_node( "a" );
    SceneNode* node_b = node_mgr->get_node( "b" );
    OK( node_a != nullptr );
    IS( root_node.get(), node_a );
    OK( node_b != nullptr );

    OK( mat_mgr->material_is_cached( "material1.json" ) );
    OK( mat_mgr->material_is_cached( "material2.json" ) );
    OK( mat_mgr->material_is_cached( "material_vert_color.json" ) );

    OK( geo_mgr->geometry_is_cached( "geom_cube.json" ) );
    OK( geo_mgr->geometry_is_cached( "geom_colored.json" ) );
    OK( geo_mgr->geometry_is_cached( "geom_simple.json" ) );

    Material* material1 = mat_mgr->get_material( "material1.json" );
    Material* material2 = mat_mgr->get_material( "material2.json" );
    Material* material_vert_color = mat_mgr->get_material( "material_vert_color.json" );

    OK( material1 != nullptr );
    OK( material2 != nullptr );
    OK( material_vert_color != nullptr );

    RefCountHolder<Geometry> geom_cube    = geo_mgr->get_geometry( "geom_cube.json" );
    RefCountHolder<Geometry> geom_colored = geo_mgr->get_geometry( "geom_colored.json" );
    RefCountHolder<Geometry> geom_simple  = geo_mgr->get_geometry( "geom_simple.json" );

    OK( geom_cube != nullptr );
    OK( geom_colored != nullptr );
    OK( geom_simple != nullptr );

    // node A children
    IS( node_a->get_num_children(), 2 );

    SceneNode* unamed_node = nullptr;
    bool       b_got       = false;
    for (int i = 0; i < node_a->get_num_children(); i++) {
        SceneNode* tmp = node_a->get_child_at( i );
        if (tmp == node_b)
            b_got = true;
        else
            unamed_node = tmp;
    }

    OK( b_got );
    OK( unamed_node );

    // node A visual item
    IS( node_a->get_num_items(),                            1 );

    VisualObject* visual_a = dynamic_cast<VisualObject*>( node_a->get_item_at( 0 ) );
    IS( visual_a->get_geometry(),                           geom_colored.get() );
    IS( static_cast<Material*>( visual_a->get_material() ), material_vert_color );

    // contents of unamed node
    IS( unamed_node->get_num_children(),                    0 );
    IS( unamed_node->get_num_items(),                       1 );

    VisualObject* visual_unamed = dynamic_cast<VisualObject*>( unamed_node->get_item_at( 0 ) );
    IS( visual_unamed->get_geometry(),                      geom_simple.get() );
    IS( visual_unamed->get_material(),                      material1 );

    // contents of node B
    IS( node_b->get_num_children(),                         0 );
    IS( node_b->get_num_items(),                            2 );

    VisualObject* visual_b1 = dynamic_cast<VisualObject*>( node_b->get_item_at( 0 ) );
    VisualObject* visual_b2 = dynamic_cast<VisualObject*>( node_b->get_item_at( 1 ) );

    if (visual_b1->get_geometry() == geom_cube)
    {
        IS( visual_b1->get_geometry(), geom_cube.get() );
        IS( visual_b1->get_material(), material2 );

        IS( visual_b2->get_geometry(), geom_colored.get() );
        IS( visual_b2->get_material(), material1 );
    }
    else
    {
        IS( visual_b1->get_geometry(), geom_colored.get() );
        IS( visual_b1->get_material(), material1 );

        IS( visual_b2->get_geometry(), geom_cube.get() );
        IS( visual_b2->get_material(), material2 );
    }

    SDL_GL_DeleteContext( context );
    SDL_Quit();
}
