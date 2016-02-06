#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include "treeface/graphics/ShapeGenerator.h"
#include "treeface/scene/MaterialManager.h"
#include "treeface/math/Constants.h"
#include "treeface/scene/Geometry.h"
#include "treeface/scene/Scene.h"
#include "treeface/scene/SceneGraphMaterial.h"
#include "treeface/scene/SceneNode.h"
#include "treeface/scene/SceneRenderer.h"
#include "treeface/scene/VisualObject.h"
#include "treeface/base/PackageManager.h"
#include <set>

#include <treecore/File.h>
#include <treecore/StringRef.h>

using namespace treeface;
using namespace treecore;
using namespace std;

const int   window_w  = 1024;
const int   window_h  = 768;
const float handle_sz = 15;

SDL_Window*   window  = nullptr;
SDL_GLContext context = nullptr;
RefCountHolder<Geometry>     curve_stroke_geom;
RefCountHolder<VisualObject> curve_stroke_visual;
RefCountHolder<Geometry>     curve_fill_geom;
RefCountHolder<VisualObject> curve_fill_visual;
RefCountHolder<VisualObject> handle_visual;
RefCountHolder<Scene>        scene;

StrokeStyle style_thick{ LINE_CAP_BUTT, LINE_JOIN_BEVEL, treeface::PI * 0.75f, 5.0f };

struct MouseHandleRect: public RefCountObject
{
    MouseHandleRect( const Vec2f& pos )
        : node( new SceneNode() )
        , position( pos )
    {
        node->add_item( handle_visual );
        _update_node_trans_();
        scene->get_root_node()->add_child( node );
    }
    virtual ~MouseHandleRect()
    {
        scene->get_root_node()->remove_child( node );
    }

    bool is_inside( Vec2f& value )
    {
        float r = handle_sz / 2;
        if (position.x - r <= value.x && value.x <= position.x + r &&
            position.y - r <= value.y && value.y <= position.y + r)
            return true;
        else
            return false;
    }

    void move( const Vec2f& delta )
    {
        position.x += delta.x;
        position.y += delta.y;
        _update_node_trans_();
    }

    void _update_node_trans_()
    {
        Mat4f trans;
        trans.set_translate( position.x, position.y, 1 );
        node->set_transform( trans );
    }

    bool  active  = false;
    bool  pressed = false;
    Vec2f position;
    RefCountHolder<SceneNode> node;
};

Array<RefCountHolder<MouseHandleRect> > mouse_handles;

void setup_sdl()
{
    SDL_Init( SDL_INIT_VIDEO & SDL_INIT_TIMER & SDL_INIT_EVENTS );

    printf( "create window\n" );
    window = SDL_CreateWindow( "sdl_setup test", 50, 50, window_w, window_h, SDL_WINDOW_OPENGL );
    if (!window)
    {
        fprintf( stderr, "error: failed to create window: %s\n", SDL_GetError() );
        exit( 1 );
    }

    printf( "create opengl context\n" );
    context = SDL_GL_CreateContext( window );
    if (!context)
    {
        fprintf( stderr, "error: failed to create GL context: %s\n", SDL_GetError() );
        exit( 1 );
    }

    SDL_GL_MakeCurrent( window, context );

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

void setup_scene()
{
    PackageManager::getInstance()->add_package( File::getCurrentWorkingDirectory().getParentDirectory().getChildFile( "examples/resource.zip" ), PackageManager::OVERWRITE );
    ShapeGenerator* gen = ShapeGenerator::getInstance();

    // create scene
    scene = new Scene();

    // create visual object for mouse handles
    float r = handle_sz / 2;
    gen->move_to( Vec2f( -r, -r ) );
    gen->line_to( Vec2f( r, -r ) );
    gen->line_to( Vec2f( r, r ) );
    gen->line_to( Vec2f( -r, r ) );
    gen->close_path();

    Geometry* geom = ShapeGenerator::create_fill_geometry();
    geom->host_draw_begin();
    gen->fill_simple( geom );
    geom->host_draw_end();

    Material* mat = scene->get_material_manager()->get_material( "material_stroke_rainbow.json" );
    if (mat == nullptr)
        die( "no material" );
    SceneGraphMaterial* mat_sg = dynamic_cast<SceneGraphMaterial*>(mat);

    handle_visual = new VisualObject( geom, mat_sg );

    // create curve
    curve_stroke_geom   = ShapeGenerator::create_complicated_stroke_geometry();
    curve_fill_geom     = ShapeGenerator::create_fill_geometry();
    curve_stroke_visual = new VisualObject( curve_stroke_geom, mat_sg );
    curve_fill_visual   = new VisualObject( curve_fill_geom, mat_sg );
    scene->get_root_node()->add_item( curve_stroke_visual );
    scene->get_root_node()->add_item( curve_fill_visual );
}

struct MouseHandleSorter
{
    int compareElements( const MouseHandleRect* a, const MouseHandleRect* b )
    {
        if (a->position.x < b->position.x)
            return -1;
        else if (a->position.x > b->position.x)
            return 1;
        else return 0;
    }
};

void update_curve()
{
    Logger::writeToLog( "update curve" );
    curve_stroke_geom->host_draw_begin();
    curve_fill_geom->host_draw_begin();

    curve_stroke_geom->get_host_vertex_cache().clear_quick();
    curve_stroke_geom->get_host_index_cache().clearQuick();
    curve_fill_geom->get_host_vertex_cache().clear_quick();
    curve_fill_geom->get_host_index_cache().clearQuick();

    if (mouse_handles.size() > 0)
    {
        Array<MouseHandleRect*> handles_sort;
        for (int i = 0; i < mouse_handles.size(); i++)
            handles_sort.add( mouse_handles[i] );

        MouseHandleSorter sorter;
        handles_sort.sort( sorter );

        ShapeGenerator* gen = ShapeGenerator::getInstance();
        gen->clear();

        Vec2f p_prev( 0.0f, handles_sort[0]->position.y );
        gen->move_to( p_prev );
        //        printf( "  move to %f %f\n", p_prev.x, p_prev.y );

        for (const MouseHandleRect* curr_handle : handles_sort)
        {
            float x_avg = (p_prev.x + curr_handle->position.x) / 2;
            Vec2f ctrl1( x_avg, p_prev.y );
            Vec2f ctrl2( x_avg, curr_handle->position.y );
            gen->curve_to( ctrl1, ctrl2, curr_handle->position );
            //            printf( "  bessel4 %f %f %f %f %f %f\n", ctrl1.x, ctrl1.y, ctrl2.x, ctrl2.y, curr_handle->position.x, curr_handle->position.y );
            p_prev = curr_handle->position;
        }

        gen->line_to( Vec2f( float(window_w), handles_sort.getLast()->position.y ) );
        //        printf( "  line %f %f\n", float(window_w), handles_sort.getLast()->position.y );

        gen->stroke_complicated_preserve( style_thick, curve_stroke_geom );

        gen->line_to( Vec2f( float(window_w), float(window_h) ) );
        gen->line_to( Vec2f( 0.0f, float(window_h) ) );
        gen->fill_simple( curve_fill_geom );
    }

    curve_stroke_geom->host_draw_end();
    curve_fill_geom->host_draw_end();
}

void on_mouse_motion( SDL_MouseMotionEvent& e )
{
    Vec2f mouse_pos( float(e.x), float(e.y) );
    Vec2f mouse_delta( float(e.xrel), float(e.yrel) );

    bool handle_moved = false;
    for (int i = 0; i < mouse_handles.size(); i++)
    {
        MouseHandleRect* curr = mouse_handles[i];
        if ( curr->is_inside( mouse_pos ) )
            curr->active = true;

        if (curr->pressed)
        {
            handle_moved = true;
            curr->move( mouse_delta );
        }
    }

    if (handle_moved) update_curve();
}

void on_mouse_down( SDL_MouseButtonEvent& e )
{
    Vec2f mouse_pos( float(e.x), float(e.y) );
    if (e.button == SDL_BUTTON_LEFT)
    {
        Logger::writeToLog( "left pressed at " + String( e.x ) + ", " + String( e.y ) );

        int num_select = 0;
        for (int i = 0; i < mouse_handles.size(); i++)
        {
            MouseHandleRect* curr = mouse_handles[i];
            if ( curr->is_inside( mouse_pos ) )
            {
                Logger::writeToLog( "  intersection with widget " + String( pointer_sized_int( curr ) ) );
                curr->pressed = true;
                num_select++;
            }
        }

        // nothing is pressed, create new widget at current position
        if (num_select == 0)
        {
            Logger::writeToLog( "  create new widget" );
            mouse_handles.add( new MouseHandleRect( mouse_pos ) );
            update_curve();
        }
    }
    else if (e.button == SDL_BUTTON_RIGHT)
    {
        Logger::writeToLog( "right pressed at " + String( e.x ) + ", " + String( e.y ) );
        for (int i = 0; i < mouse_handles.size(); i++)
        {
            MouseHandleRect* curr = mouse_handles[i];
            if ( curr->is_inside( mouse_pos ) )
            {
                Logger::writeToLog( "  remove widget " + String( pointer_sized_int( curr ) ) );
                mouse_handles.remove( i );
                i--;
            }
        }

        update_curve();
    }
}

void on_mouse_up( SDL_MouseButtonEvent& e )
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        Logger::writeToLog( "left released" );
        for (int i = 0; i < mouse_handles.size(); i++)
            mouse_handles[i]->pressed = false;
    }
}

#undef main
int main( int argc, char** argv )
{
    setup_sdl();
    setup_scene();

    SceneRenderer renderer;

    Mat4f mat_view;
    mat_view.set_scale( 2.0f / window_w, -2.0f / window_h, 0.01 );
    mat_view.set_translate( -1.0f, 1.0f, 0.0f );

    // main loop
    while (1)
    {
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
            case SDL_MOUSEMOTION:
                on_mouse_motion( event.motion );
                break;
            case SDL_MOUSEBUTTONDOWN:
                on_mouse_down( event.button );
                break;
            case SDL_MOUSEBUTTONUP:
                on_mouse_up( event.button );
                break;
            }

            if (event.type == SDL_QUIT)
            {
                should_exit = true;
            }
        }

        if (should_exit) break;

        // do render
        glClear( GL_COLOR_BUFFER_BIT );

        renderer.render( mat_view, Mat4f(), scene );

        SDL_GL_SwapWindow( window );
        SDL_Delay( 20 );
    }

    SDL_GL_DeleteContext( context );
    SDL_Quit();

    return 0;
}
