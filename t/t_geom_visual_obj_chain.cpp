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
    RefCountHolder<Geometry>           geom1    = geom_mgr->get_geometry( "geom_sphere1.json" );
    RefCountHolder<Geometry>           geom2    = geom_mgr->get_geometry( "geom_sphere2.json" );
    RefCountHolder<SceneGraphMaterial> mat      = dynamic_cast<SceneGraphMaterial*>( mat_mgr->get_material( "material_uni_color.json" ) );

    OK( geom1->m_impl->user_head == nullptr );
    OK( geom1->m_impl->user_tail == nullptr );

    RefCountHolder<VisualObject> obj1 = new VisualObject( geom1, mat );
    RefCountHolder<VisualObject> obj2 = new VisualObject( geom1, mat );
    RefCountHolder<VisualObject> obj3 = new VisualObject( geom2, mat );

    // everything should be initially empty
    obj1->m_impl->update_uniform_cache();
    obj2->m_impl->update_uniform_cache();
    obj3->m_impl->update_uniform_cache();
    OK( !obj1->m_impl->uniform_cache_dirty );
    OK( !obj2->m_impl->uniform_cache_dirty );
    OK( !obj3->m_impl->uniform_cache_dirty );
    IS( obj1->m_impl->cached_uniforms.size(), 0 );
    IS( obj2->m_impl->cached_uniforms.size(), 0 );
    IS( obj3->m_impl->cached_uniforms.size(), 0 );

    // set uniform value on geometry
    geom1->set_uniform_value( "color", Vec4f( 1.0f, 0.0f, 0.0f, 1.0f ) );
    OK( obj1->m_impl->uniform_cache_dirty );
    OK( obj2->m_impl->uniform_cache_dirty );
    OK( !obj3->m_impl->uniform_cache_dirty );

    obj1->m_impl->update_uniform_cache();
    obj2->m_impl->update_uniform_cache();

    IS( obj1->m_impl->cached_uniforms.size(), 1 );
    IS( obj2->m_impl->cached_uniforms.size(), 1 );
    IS( obj3->m_impl->cached_uniforms.size(), 0 );

    OK( Vec4f( obj1->m_impl->cached_uniforms[0].second ) == Vec4f( 1.0f, 0.0f, 0.0f, 1.0f ) );
    OK( Vec4f( obj2->m_impl->cached_uniforms[0].second ) == Vec4f( 1.0f, 0.0f, 0.0f, 1.0f ) );

    // set uniform value on visual object
    // this should override geometry's value
    obj2->set_uniform_value( "color", Vec4f( 1.0f, 1.0f, 1.0f, 1.0f ) );
    OK( !obj1->m_impl->uniform_cache_dirty );
    OK( obj2->m_impl->uniform_cache_dirty );
    OK( !obj3->m_impl->uniform_cache_dirty );

    obj2->m_impl->update_uniform_cache();

    IS( obj2->m_impl->cached_uniforms.size(), 1 );
    OK( Vec4f( obj2->m_impl->cached_uniforms[0].second ) == Vec4f( 1.0f, 1.0f, 1.0f, 1.0f ) );

    // set non-exist uniform value
    geom2->set_uniform_value( "foobar", 1.0f );
    OK( !obj1->m_impl->uniform_cache_dirty );
    OK( !obj2->m_impl->uniform_cache_dirty );
    OK( obj3->m_impl->uniform_cache_dirty );

    obj3->m_impl->update_uniform_cache();
    IS( obj3->m_impl->cached_uniforms.size(), 0 );
}
