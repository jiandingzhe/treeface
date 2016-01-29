#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/scene/Material.h"
#include "treeface/scene/MaterialManager.h"
#include "treeface/base/PackageManager.h"

#include "treeface/scene/Geometry.h"
#include "treeface/scene/GeometryManager.h"
#include "treeface/scene/Scene.h"
#include "treeface/scene/SceneNode.h"
#include "treeface/scene/SceneNodeManager.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/SceneRenderer.h"

#include "treeface/gl/Program.h"
#include "treeface/gl/Texture.h"
#include "treeface/gl/VertexArray.h"

#include "treeface/math/Mat4.h"
#include "treeface/math/Quat.h"

#include <treecore/File.h>
#include <treecore/RefCountHolder.h>
#include <treecore/JSON.h>
#include <treecore/MemoryBlock.h>
#include <treecore/StringRef.h>
#include <treecore/Variant.h>

using namespace treeface;
using namespace treecore;

#define NODE_HIER_NAME "nodes.json"

int window_w = 400;
int window_h = 400;
void build_up_sdl( SDL_Window** window, SDL_GLContext* context )
{
    SDL_Init( SDL_INIT_VIDEO & SDL_INIT_TIMER & SDL_INIT_EVENTS );

    printf( "create window\n" );
    *window = SDL_CreateWindow( "sdl_setup test", 50, 50, window_w, window_h, SDL_WINDOW_OPENGL );
    if (!*window)
    {
        fprintf( stderr, "error: failed to create window: %s\n", SDL_GetError() );
        exit( 1 );
    }

    printf( "create opengl context\n" );
    *context = SDL_GL_CreateContext( *window );
    if (!context)
    {
        fprintf( stderr, "error: failed to create GL context: %s\n", SDL_GetError() );
        exit( 1 );
    }

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

RefCountHolder<GeometryManager>  geo_mgr;
RefCountHolder<MaterialManager>  mat_mgr;
RefCountHolder<SceneNodeManager> node_mgr;
PackageManager* pkg_mgr = nullptr;

RefCountHolder<Scene> scene;

void build_up_gl()
{
    glEnable( GL_DEPTH_TEST );

    geo_mgr  = new GeometryManager();
    mat_mgr  = new MaterialManager();
    node_mgr = new SceneNodeManager( geo_mgr, mat_mgr );
    pkg_mgr  = PackageManager::getInstance();

    File pkg_file = File::getCurrentWorkingDirectory().getChildFile( "../examples/resource.zip" );
    Logger::writeToLog( "load package from file " + pkg_file.getFullPathName() );
    pkg_mgr->add_package( pkg_file, PackageManager::USE_NEWER );

    // parse node hierarchy
    MemoryBlock node_hie_src;
    if ( !pkg_mgr->get_item_data( NODE_HIER_NAME, node_hie_src, true ) )
        die( "no resource named \"%s\"", NODE_HIER_NAME );

    var json_root_node;
    {
        Result re = JSON::parse( node_hie_src.toString(), json_root_node );
        if (!re)
            die( "%s", re.getErrorMessage().toRawUTF8() );
    }

    RefCountHolder<SceneNode> builded_nodes = node_mgr->add_nodes(json_root_node);

    // create scene
    scene = new Scene();
    scene->get_root_node()->add_child( builded_nodes );
}

void main_loop( SDL_Window* window )
{
    RefCountHolder<SceneRenderer> renderer = new SceneRenderer();

    float angle = 0;

    while (1)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // process events
        SDL_Event event{};
        bool      should_exit = false;

        while ( SDL_PollEvent( &event ) )
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

        renderer->render( Mat4f(), Mat4f(), scene );

        // finalize
        SDL_GL_SwapWindow( window );
        SDL_Delay( 50 );
    }
}

#undef main
int main( int argc, char** argv )
{
    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl( &window, &context );
    build_up_gl();

    main_loop( window );

    SDL_GL_DeleteContext( context );
    SDL_Quit();

    return 0;
}
