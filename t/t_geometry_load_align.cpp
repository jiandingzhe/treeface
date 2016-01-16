#include "TestFramework.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/gl/vertextemplate.h"
#include "treeface/packagemanager.h"
#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"

#include <treecore/File.h>
#include <treecore/StringRef.h>
#include <treecore/MemoryBlock.h>

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
    RefCountHolder<GeometryManager> geom_mgr = new GeometryManager();

    pkg_mgr->add_package( File( "../examples/resource.zip" ), PackageManager::KEEP_EXISTING );
    OK( pkg_mgr->has_resource( "geom_with_align.json" ) );

    RefCountHolder<Geometry> geom = geom_mgr->get_geometry( "geom_with_align.json" );
    OK( geom );

    IS( geom->get_primitive(), GL_TRIANGLE_STRIP );
    IS(geom->get_vertex_template().vertex_size(), 24);

    {
        Geometry::HostDrawScope    scope( *geom );
        Geometry::HostVertexCache& data_vtx = geom->get_host_vertex_cache();
        GLbyte* data_p = (GLbyte*) data_vtx.get_raw_data_ptr();

        IS( data_vtx.size(),         4 );

        OK("vertex 0");
        IS( *(float*) (data_p + 0),  -0.5f );
        IS( *(float*) (data_p + 4),  -0.5f );
        IS( *(float*) (data_p + 8),  0.0f );

        IS( *(float*) (data_p + 16), 0.0f );
        IS( *(float*) (data_p + 20), 0.0f );

        OK("vertex 1");
        IS( *(float*) (data_p + 24), 0.5f );
        IS( *(float*) (data_p + 28), -0.5f );
        IS( *(float*) (data_p + 32), 0.0f );

        IS( *(float*) (data_p + 40), 1.0f );
        IS( *(float*) (data_p + 44), 0.0f );

        OK("vertex 2");
        IS( *(float*) (data_p + 48), -0.5f );
        IS( *(float*) (data_p + 52), 0.5f );
        IS( *(float*) (data_p + 56), 0.0f );

        IS( *(float*) (data_p + 64), 0.0f );
        IS( *(float*) (data_p + 68), 1.0f );

        OK("vertex 3");
        IS( *(float*) (data_p + 72), 0.5f );
        IS( *(float*) (data_p + 76), 0.5f );
        IS( *(float*) (data_p + 80), 0.0f );

        IS( *(float*) (data_p + 88), 1.0f );
        IS( *(float*) (data_p + 92), 1.0f );

        Array<IndexType>& data_idx = geom->get_host_index_cache();
        IS( data_idx.size(),         4 );
        IS( data_idx[0],             0 );
        IS( data_idx[1],             1 );
        IS( data_idx[2],             2 );
        IS( data_idx[3],             3 );
    }

    SDL_GL_DeleteContext( context );
    SDL_Quit();
}
