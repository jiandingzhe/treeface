#include "TestFramework.h"

#include "treeface/graphics/ImageManager.h"
#include "treeface/scene/Material.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/MaterialManager.h"
#include "treeface/base/PackageManager.h"

#include <treecore/File.h>
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
    printf( "glew inited\n" );
}

void TestFramework::content()
{
    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl( &window, &context );

    PackageManager* pkg_mgr = PackageManager::getInstance();
    ImageManager*   img_mgr = ImageManager::getInstance();

    RefCountHolder<MaterialManager> mat_mgr = new MaterialManager();

    printf( "add zip file to package manager\n" );
    pkg_mgr->add_package( File::getCurrentWorkingDirectory().getChildFile( "../examples/resource.zip" ), PackageManager::KEEP_EXISTING );

    // material 1
    printf( "load material 1\n" );
    {
        Material* mat1 = mat_mgr->get_material( "material1.json" );
        OK( mat1 != nullptr );
        IS( mat1->get_ref_count(),    1 );

        IS( mat1->get_num_textures(), 1 );
        OK( img_mgr->image_is_cached( "moon.jpg" ) );
        SceneGraphMaterial* scene_mat1 = dynamic_cast<SceneGraphMaterial*>(mat1);
        OK( scene_mat1 != nullptr );
    }

    // material 2
    {
        Material* mat2 = mat_mgr->get_material( "material2.json" );
        OK( mat2 != nullptr );
        IS( mat2->get_ref_count(),    1 );

        IS( mat2->get_num_textures(), 2 );
        OK( img_mgr->image_is_cached( "earth.png" ) );
    }

    // suppress valgrind warnings
    PackageManager::releaseInstance();
    ImageManager::releaseInstance();

    SDL_GL_DeleteContext( context );
    SDL_Quit();
}
