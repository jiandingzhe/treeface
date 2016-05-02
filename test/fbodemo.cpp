#include "treecore/File.h"

#include "treeface/base/PackageManager.h"

#include "treeface/gl/Framebuffer.h"
#include "treeface/gl/Texture.h"

#include "treeface/graphics/ShapeGenerator.h"

#include "treeface/math/Constants.h"

#include "treeface/scene/Geometry.h"
#include "treeface/scene/GeometryManager.h"
#include "treeface/scene/Material.h"
#include "treeface/scene/MaterialManager.h"
#include "treeface/scene/Scene.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/SceneNode.h"
#include "treeface/scene/SceneRenderer.h"
#include "treeface/scene/VisualObject.h"

#include "FreeImage.h"

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

int main()
{
    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl( &window, &context );

    PackageManager* pkg_mgr = PackageManager::getInstance();
    pkg_mgr->add_package( File::getCurrentWorkingDirectory().getChildFile( "../examples/resource.zip" ), PackageManager::KEEP_EXISTING );

    RefCountHolder<GeometryManager>    geo_mgr = new GeometryManager();
    RefCountHolder<MaterialManager>    mat_mgr = new MaterialManager();
    RefCountHolder<SceneGraphMaterial> mat     = dynamic_cast<SceneGraphMaterial*>( mat_mgr->get_material( "material_stroke_rainbow.json" ) );

    // create 2D plotting
    RefCountHolder<Geometry> geo_2d = new Geometry( ShapeGenerator::VERTEX_TEMPLATE_STROKE(), TFGL_PRIMITIVE_TRIANGLES, true );

    ShapeGenerator* gen = ShapeGenerator::getInstance();
    gen->move_to( Vec2f( -1.0f, -1.0f ) );
    gen->line_to( Vec2f( -0.7f, 0.7f ) );
    gen->line_to( Vec2f( 0.5f, 0.5f ) );
    gen->line_to( Vec2f( 0.6f, -0.4f ) );

    geo_2d->host_draw_begin();
    gen->stroke_complicated( StrokeStyle{ LINE_CAP_ROUND, LINE_JOIN_ROUND, treeface::PI * 0.75f, 0.05f }, geo_2d );
    geo_2d->host_draw_end();

    // create FBO
    RefCountHolder<Texture>     tex_out_rgba  = new Texture( window_w, window_h, 1, TFGL_INTERNAL_IMAGE_FORMAT_RGBA8 );
    RefCountHolder<Texture>     tex_out_depth = new Texture( window_w, window_h, 1, TFGL_INTERNAL_IMAGE_FORMAT_DEPTH16 );
    RefCountHolder<Framebuffer> fbo = new Framebuffer();
    fbo->attach_2d_texture( TFGL_FB_ATTACH_COLOR0, tex_out_rgba );
    fbo->attach_2d_texture( TFGL_FB_ATTACH_DEPTH,  tex_out_depth );

    // build up scene
    RefCountHolder<Scene> scene = new Scene( geo_mgr, mat_mgr );
    RefCountHolder<VisualObject> vis_obj = new VisualObject( geo_2d, mat );

    scene->get_root_node()->add_item( vis_obj );

    // do render
    RefCountHolder<SceneRenderer> render = new SceneRenderer();
    treecore_assert( fbo->is_bound_to_draw() );
    render->render( Mat4f(), Mat4f(), scene );

    // fetch texture image
    FIBITMAP* img = FreeImage_Allocate( window_w, window_h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000 );
    tex_out_rgba->bind();

    glGetTexImage( TFGL_TEXTURE_2D, 0, TFGL_IMAGE_FORMAT_RGBA, TFGL_IMAGE_DATA_UNSIGNED_BYTE, FreeImage_GetBits( img ) );

    // write output
    FreeImage_Save( FIF_PNG, img, "fbodemo.png" );
}
