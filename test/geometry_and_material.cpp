#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/material.h"
#include "treeface/materialmanager.h"
#include "treeface/packagemanager.h"

#include "treeface/scene/geometry.h"
#include "treeface/scene/geometrymanager.h"
#include "treeface/scene/scenegraphmaterial.h"

#include "treeface/gl/glbuffer.h"
#include "treeface/gl/program.h"
#include "treeface/gl/texture.h"
#include "treeface/gl/vertexarray.h"

#include "treeface/math/mat4.h"
#include "treeface/math/quat.h"

#include <treecore/File.h>
#include <treecore/RefCountHolder.h>
#include <treecore/StringRef.h>

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

RefCountHolder<GeometryManager> geo_mgr;
RefCountHolder<MaterialManager> mat_mgr;
PackageManager* pkg_mgr = nullptr;
RefCountHolder<VertexArray> vao_a = nullptr;
RefCountHolder<VertexArray> vao_b = nullptr;

String name_geo_a( "geom_cube.json" );
String name_geo_b( "geom_colored.json" );
String name_mat_a( "material1.json" );
String name_mat_b( "material2.json" );

RefCountHolder<Geometry> geo_a = nullptr;
RefCountHolder<Geometry> geo_b = nullptr;
RefCountHolder<SceneGraphMaterial> mat_a = nullptr;
RefCountHolder<SceneGraphMaterial> mat_b = nullptr;

void build_up_gl()
{
    glEnable( GL_DEPTH_TEST );

    geo_mgr = new GeometryManager();
    mat_mgr = new MaterialManager();
    pkg_mgr = PackageManager::getInstance();

    String pkg_file_name( "../examples/resource.zip" );
    Logger::writeToLog( "load package from file " + pkg_file_name );
    pkg_mgr->add_package( File( pkg_file_name ), PackageManager::USE_NEWER );

    geo_a = geo_mgr->get_geometry( name_geo_a );
    if (!geo_a)
        die( "no geometry named \"%s\"", name_geo_a.toRawUTF8() );

    geo_b = geo_mgr->get_geometry( name_geo_b );
    if (!geo_b)
        die("no geometry named \"%s\"", name_geo_b.toRawUTF8());

    // manually upload geometry data
    {
        geo_a->get_vertex_buffer()->bind();
        geo_a->get_index_buffer()->bind();
        geo_a->upload_data();
        geo_a->get_vertex_buffer()->unbind();
        geo_a->get_index_buffer()->unbind();

        geo_b->get_vertex_buffer()->bind();
        geo_b->get_index_buffer()->bind();
        geo_b->upload_data();
        geo_b->get_vertex_buffer()->unbind();
        geo_b->get_index_buffer()->unbind();
    }

    // create material
    {
        Material* mat = mat_mgr->get_material( name_mat_a );
        if (mat == nullptr)
            die( "no material named \"%s\"", name_mat_a.toRawUTF8() );

        mat_a = dynamic_cast<SceneGraphMaterial*>(mat);
        if (!mat_a)
            die( "material \"%s\" is not a scene graph material", name_mat_a.toRawUTF8() );
    }

    {
        Material* mat = mat_mgr->get_material( name_mat_b );
        if (!mat)
            die( "no material named \"%s\"", name_mat_b.toRawUTF8() );

        mat_b = dynamic_cast<SceneGraphMaterial*>(mat);
        if (!mat_b)
            die( "material \"%s\" is not a scene graph material", name_mat_b.toRawUTF8() );
    }

    // create vertex array, bind vertex attributes
    vao_a = new VertexArray( geo_a->get_vertex_buffer(), geo_a->get_index_buffer(), geo_a->get_vertex_template(), mat_a->get_program() );
    vao_b = new VertexArray( geo_b->get_vertex_buffer(), geo_b->get_index_buffer(), geo_b->get_vertex_template(), mat_b->get_program() );
}

void main_loop( SDL_Window* window )
{
    Mat4f model_view_mat;
    Mat4f scale_3x;
    scale_3x.set_scale( 3, 3, 3 );

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

        angle += 0.02;
        Quatf rot( angle, Vec4f( 1, 1, 0, 0 ) );
        model_view_mat.set_rotate( rot );

        // draw A
        {
            mat_a->bind();

            mat_a->set_matrix_model_view( model_view_mat );
            mat_a->set_matrix_proj( Mat4f() );
            mat_a->set_matrix_model_view_proj( model_view_mat );

            vao_a->bind();

            vao_a->draw( geo_a->get_primitive(), geo_a->get_num_index() );

            mat_a->unbind();
            vao_a->unbind();
        }

        // draw B
        {
            mat_b->bind();
            mat_b->set_matrix_model_view( scale_3x );
            mat_b->set_matrix_proj( Mat4f() );
            mat_b->set_matrix_model_view_proj( scale_3x );

            vao_b->bind();

            vao_b->draw( geo_a->get_primitive(), geo_a->get_num_index() );

            mat_b->unbind();
            vao_b->unbind();
        }

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
