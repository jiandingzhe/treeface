#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/material.h"
#include "treeface/materialmanager.h"
#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/scene.h"
#include "treeface/scene/scenenode.h"
#include "treeface/scene/scenenodemanager.h"
#include "treeface/scene/scenegraphmaterial.h"
#include "treeface/scene/scenerenderer.h"
#include "treeface/scene/visualobject.h"

#include "treeface/gl/program.h"
#include "treeface/gl/texture.h"
#include "treeface/gl/vertexarray.h"

#include "treeface/math/mat4.h"
#include "treeface/math/quat.h"

#include <treecore/File.h>
#include <treecore/RefCountHolder.h>
#include <treecore/JSON.h>
#include <treecore/Result.h>
#include <treecore/StringRef.h>
#include <treecore/Variant.h>

using namespace treeface;
using namespace treecore;

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

#define GEOM_FRONT     "geom_sphere2.json"
#define GEOM_BACK      "geom_simple.json"
#define MATERIAL_FRONT "material_color_light.json"
#define MATERIAL_BACK  "material_plain.json"

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

    // create scene
    scene = new Scene( geo_mgr, mat_mgr );
    scene->set_global_light_ambient( 0.2, 0.2, 0.2, 1 );
    scene->set_global_light_direction( 1, 1, -0.5 );

    RefCountHolder<Geometry> geom      = geo_mgr->get_geometry( GEOM_FRONT );
    RefCountHolder<Geometry> geom_back = geo_mgr->get_geometry( GEOM_BACK );

    if (!geom)
        die( "failed to get geometry \"%s\"", GEOM_FRONT );

    if (!geom_back)
        die( "failed to get geometry \"%s\"", GEOM_BACK );

    RefCountHolder<Material> mat      = mat_mgr->get_material( MATERIAL_FRONT );
    RefCountHolder<Material> mat_back = mat_mgr->get_material( MATERIAL_BACK );
    if (!mat)
        die( "no material named %s\n", MATERIAL_FRONT );
    if (!mat_back)
        die( "no material named %s\n", MATERIAL_BACK );

    VisualObject* item = nullptr;
    {
        SceneGraphMaterial* mat_sg = dynamic_cast<SceneGraphMaterial*>( mat.get() );
        if (mat_sg == nullptr)
            die( "material \"%s\" is not a scene graph material", MATERIAL_FRONT );
        printf( "create visual object %p with geometry %p and material %p\n", item, geom.get(), mat_sg );
        item = new VisualObject( geom, mat_sg );
    }
    SceneNode* node = new SceneNode();
    node->add_item( item );
    scene->get_root_node()->add_child( node );

    VisualObject* item_back = nullptr;
    {
        SceneGraphMaterial* mat_sg = dynamic_cast<SceneGraphMaterial*>( mat_back.get() );
        if (mat_sg == nullptr)
            die( "material \"%s\" is not a scene graph material", MATERIAL_BACK );
        printf( "build background object %p with geometry %p and material %p\n", item_back, geom_back.get(), mat_sg );
        item_back = new VisualObject( geom_back, mat_sg );
    }
    scene->get_root_node()->add_item( item_back );
}

void main_loop( SDL_Window* window )
{
    SceneNode* node = scene->get_root_node()->get_child_at( 0 );

    RefCountHolder<SceneRenderer> renderer = new SceneRenderer();

    float angle = 0;
    Mat4f mat;

    Mat4f view;
    view.set_scale( 0.5, 0.5, 0.5 );

    Mat4f proj;
    proj.set_perspective( PI / 4, 1, 0.01, 1 );

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

        mat.set_rotate( Quatf( angle, Vec4f( 1, -1, 0, 0 ) ) );
        node->set_transform( mat );
        renderer->render( proj, view, scene );

        // finalize
        glFinish();
        SDL_GL_SwapWindow( window );
        SDL_Delay( 50 );
        angle += 0.1;
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
