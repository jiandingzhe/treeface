#include "TestFramework.h"

#include "treecore/File.h"

#include "treeface/base/PackageManager.h"
#include "treeface/scene/Geometry.h"
#include "treeface/scene/GeometryManager.h"
#include "treeface/scene/Material.h"
#include "treeface/scene/MaterialManager.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/VisualObject.h"

#include "treeface/scene/guts/Geometry_guts.h"
#include "treeface/scene/guts/VisualObject_guts.h"

#define GLEW_STATIC
#include <GL/glew.h>
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
    pkg_mgr->add_package( File::getCurrentWorkingDirectory().getChildFile( "../examples/resource.zip" ), PackageManager::KEEP_EXISTING );

    RefCountHolder<GeometryManager>    geom_mgr = new GeometryManager();
    RefCountHolder<MaterialManager>    mat_mgr  = new MaterialManager();
    RefCountHolder<Geometry>           geom     = geom_mgr->get_geometry( "geom_sphere1.json" );
    RefCountHolder<SceneGraphMaterial> mat      = dynamic_cast<SceneGraphMaterial*>( mat_mgr->get_material( "material_plain.json" ) );

    OK( geom->m_impl->user_head == nullptr );
    OK( geom->m_impl->user_tail == nullptr );

    RefCountHolder<VisualObject> obj1 = new VisualObject( geom, mat );
    IS( geom->m_impl->user_head, obj1->m_impl );
    OK( obj1->m_impl->same_geom_prev == nullptr );
    OK( obj1->m_impl->same_geom_next == nullptr );
    IS( geom->m_impl->user_tail, obj1->m_impl );

    RefCountHolder<VisualObject> obj2 = new VisualObject( geom, mat );
    IS( geom->m_impl->user_head, obj1->m_impl );
    OK( obj1->m_impl->same_geom_prev == nullptr );
    IS( obj1->m_impl->same_geom_next, obj2->m_impl );
    IS( obj2->m_impl->same_geom_prev, obj1->m_impl );
    OK( obj2->m_impl->same_geom_next == nullptr );
    IS( geom->m_impl->user_tail, obj2->m_impl );

    RefCountHolder<VisualObject> obj3 = new VisualObject( geom, mat );
    IS( geom->m_impl->user_head, obj1->m_impl );
    IS( geom->m_impl->user_head, obj1->m_impl );
    OK( obj1->m_impl->same_geom_prev == nullptr );
    IS( obj1->m_impl->same_geom_next, obj2->m_impl );
    IS( obj2->m_impl->same_geom_prev, obj1->m_impl );
    IS( obj2->m_impl->same_geom_next, obj3->m_impl );
    IS( obj3->m_impl->same_geom_prev, obj2->m_impl );
    OK( obj3->m_impl->same_geom_next == nullptr );
    IS( geom->m_impl->user_tail, obj3->m_impl );

    obj2 = nullptr;
    IS( geom->m_impl->user_head, obj1->m_impl );
    OK( obj1->m_impl->same_geom_prev == nullptr );
    IS( obj1->m_impl->same_geom_next, obj3->m_impl );
    IS( obj3->m_impl->same_geom_prev, obj1->m_impl );
    OK( obj3->m_impl->same_geom_next == nullptr );
    IS( geom->m_impl->user_tail, obj3->m_impl );

    obj1 = nullptr;
    IS( geom->m_impl->user_head, obj3->m_impl );
    OK( obj3->m_impl->same_geom_prev == nullptr );
    OK( obj3->m_impl->same_geom_next == nullptr );
    IS( geom->m_impl->user_tail, obj3->m_impl );

    obj3 = nullptr;
    OK( geom->m_impl->user_head == nullptr );
    OK( geom->m_impl->user_tail == nullptr );
}
